package mr.io;
import java.io.File;

public class LocalFileNames {
	public static String[] getFileNames(final String localpath) throws Exception
	{
		File file=new File(localpath);
		File[] files=file.listFiles();
		String[] filenames=new String[files.length];
		int i=0;
		for(File singleFile : files)
		{
			filenames[i]=singleFile.getAbsolutePath();
			++i;
		}
		return filenames;
	}
}