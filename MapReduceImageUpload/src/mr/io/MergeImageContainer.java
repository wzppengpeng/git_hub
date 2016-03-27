package mr.io;

import java.io.ByteArrayOutputStream;

public class MergeImageContainer {
	public static byte[] Merge(ImageContainer img) throws Exception
	{
		ByteArrayOutputStream arrayOutputStream=new ByteArrayOutputStream();
		arrayOutputStream.write(img.height);
		arrayOutputStream.write(img.width);
		arrayOutputStream.write(img.data);
		return arrayOutputStream.toByteArray();
	}

}