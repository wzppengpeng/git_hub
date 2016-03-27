package mr.mapreduce;



import mr.io.*;

public class GetImageFromSequenceFiles {
	public static ImageContainer getImage(byte[] imageBytes)
	{
		int height=imageBytes[0];
		int width=imageBytes[1];
		
		byte[] data=new byte[imageBytes.length-2];
		for(int i=0;i<data.length;++i)
		{
			data[i]=imageBytes[i+2];
		}
		ImageContainer imageContainer=new ImageContainer();
		imageContainer.height=height;
		imageContainer.width=width;
		imageContainer.data=data;
		return imageContainer;
	}
	
	public static String tostring(Result re)
	{
		String contents;
		String tmp_label=String.valueOf(re.label);
		String tmp_score=String.valueOf(re.score);
		contents=tmp_label+":"+tmp_score;
		return contents;
	}
}
