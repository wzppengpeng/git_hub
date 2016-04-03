package mr.mapreduce;

import java.io.*;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.BytesWritable;
import org.apache.hadoop.io.IOUtils;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.SequenceFile;
import org.apache.hadoop.io.SequenceFile.Reader;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.mapreduce.lib.output.TextOutputFormat;

import mr.io.*;
import mr.jni.*;

public class ImageRecognitionDriver {
	private static final String Root_path="hdfs://localhost:9000/user/wzp/";
	
	public static class ImageRecognitionMapper extends Mapper<Text, IntWritable, Text, Text>
	{
		private Image_jni image_jni=new Image_jni();
		
		public void map(Text key,IntWritable value,Context context)throws IOException, InterruptedException 
		{
			//initialize the CNN-net
			image_jni.initialize();
			
			Configuration conf=new Configuration();
			String SequenceFilePath=key.toString();//the hdfs path of sequence image files
			Path path=new Path(SequenceFilePath);
	
			//read contents of sequencefiles
			SequenceFile.Reader reader=new SequenceFile.Reader(conf, Reader.file(path));
			Text key_temp=new Text();//the image absolutaly path
			BytesWritable value_temp=new BytesWritable();//the imageBytes
			
			//get images
			while(reader.next(key_temp,value_temp))
			{
				byte[] tempBytes=value_temp.copyBytes();
				
				ImageContainer img=GetImageFromSequenceFiles.getImage(tempBytes);
				Result re=new Result();
				image_jni.predictImage(img, re);
				String content=GetImageFromSequenceFiles.tostring(re);
				
				context.write(key_temp, new Text(content));
			}
			IOUtils.closeStream(reader);
		}
	}
	
	public static void main(String[] args) throws Exception {
		// TODO Auto-generated method stub

		Configuration conf=new Configuration();
		Job job=Job.getInstance(conf,"Image_recognition");
		
		job.setJarByClass(ImageRecognitionDriver.class);
		job.setMapperClass(ImageRecognitionMapper.class);
		
		job.setInputFormatClass(ImageInputFormat.class);
		job.setOutputFormatClass(TextOutputFormat.class);
		
		job.setOutputKeyClass(Text.class);
		job.setOutputValueClass(Text.class);
		
		FileInputFormat.setInputPaths(job, new Path(Root_path+"files"));
		FileOutputFormat.setOutputPath(job, new Path(Root_path+"result"));
		
		System.exit(job.waitForCompletion(true) ? 0 : 1);
		
	}

}
