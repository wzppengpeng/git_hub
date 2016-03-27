package mr.io;

import java.io.IOException;

import org.apache.hadoop.fs.Path;

import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.InputSplit;
import org.apache.hadoop.mapreduce.RecordReader;
import org.apache.hadoop.mapreduce.TaskAttemptContext;
import org.apache.hadoop.mapreduce.lib.input.FileSplit;

public class ImageRecordReader extends RecordReader<Text, IntWritable>{

	private String m_fileHDFS=null;
	private int internal=10;
	private boolean m_isProcessed = false;
	
	 public ImageRecordReader(InputSplit split, TaskAttemptContext context) {
		// TODO Auto-generated constructor stub
		try{
			initialize(split, context);
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
	}
	@Override
	public void close() throws IOException {
		// TODO Auto-generated method stub
		
	}

	@Override
	public Text getCurrentKey() throws IOException, InterruptedException {
		
		return new Text(m_fileHDFS);
	}

	@Override
	public IntWritable getCurrentValue() throws IOException, InterruptedException {
		
		return new IntWritable(internal);
	}

	@Override
	public float getProgress() throws IOException, InterruptedException {
		
		return (m_isProcessed ? 0.f : 1.f);
	}

	@Override
	public void initialize(InputSplit split, TaskAttemptContext context) throws IOException, InterruptedException {
		
		FileSplit fileSplit=(FileSplit)split;
		Path filepath=fileSplit.getPath();
		m_fileHDFS=filepath.toString();
		
	}

	@Override
	public boolean nextKeyValue() throws IOException, InterruptedException {
		
		if (!m_isProcessed) {
			m_isProcessed = true;
			return true;
		}
		return false;
	}
	
	
	

}
