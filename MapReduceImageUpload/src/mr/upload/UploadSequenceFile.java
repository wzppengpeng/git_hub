package mr.upload;

import org.apache.hadoop.conf.Configuration;
//import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.BytesWritable;
import org.apache.hadoop.io.IOUtils;

import org.apache.hadoop.io.SequenceFile;
import org.apache.hadoop.io.SequenceFile.CompressionType;
import org.apache.hadoop.io.SequenceFile.Writer;
import org.apache.hadoop.io.Text;

import mr.io.ImageContainer;
import mr.io.LoadImage;
import mr.io.MergeImageContainer;
import mr.io.LocalFileNames;;

public class UploadSequenceFile {
	private static final String Root_path="/user/wzp/files/";
	private static final String Local_root_path="/home/wzp/Documents/imageSet/samples";
	private static final int interval=10;
	
	private static BytesWritable ReadFileToBytesWritable(String filename)throws Exception
	{
		ImageContainer img=LoadImage.loadImageToByteArray(filename);
		byte[] imageBytes=MergeImageContainer.Merge(img);
		BytesWritable BytesValue=new BytesWritable(imageBytes);
		return BytesValue;
	}
	
	private static void upload() throws Exception
	{
		String[] filenames=LocalFileNames.getFileNames(Local_root_path);
		
		//to get SequenseFiles;
		int length=filenames.length;
		int times;
		
		if((length%interval)==0)
		{
			times=(int)(length/interval);
		}
		else {
			times=(int)(length/interval)+1;
		}
		
		
		for(int time=0;time<times;++time)
		{
			uploadOne(filenames, time);
		}
		
		System.out.println("Upload OK");
	}
	
	private static void uploadOne(String[] filenames,int time)throws Exception
	{
		Configuration conf=new Configuration();
		//FileSystem fileSystem=FileSystem.get(conf);
		
		Path path=new Path(Root_path+"file"+time);
		SequenceFile.Writer writer=SequenceFile.createWriter(conf,Writer.file(path),Writer.keyClass(Text.class),Writer.valueClass(BytesWritable.class),Writer.compression(CompressionType.RECORD));
		int begin=time*interval;
		int end=(time+1)*interval;
		
		for(int i=begin;i<end;++i)
		{
			
			BytesWritable bytesWritable=ReadFileToBytesWritable(filenames[i]);
			writer.append(new Text(filenames[i]), bytesWritable);
			if(i==filenames.length-1)
			{
				break;
			}
		}
		IOUtils.closeStream(writer);
		
	}
	public static void main(String[] args)throws Exception {
		// TODO Auto-generated method stub
		upload();
	}

}
