package mr.io;
import javax.imageio.*;

import java.awt.image.BufferedImage;
import java.io.*;

public class LoadImage {
	public static ImageContainer loadImageToByteArray(String imagePath) {
		File f = new File(imagePath);
		BufferedImage image = null;
		try {
			image = ImageIO.read(f);
		} catch (Exception e) {

		}
		int height = image.getHeight();
		int width = image.getWidth();
		int[] pixes = new int[height * width];
		image.getRGB(0, 0, width, height, pixes, 0, width);
		byte[] data_ = readPixesToByteArray(pixes);

		ImageContainer img = new ImageContainer();
		img.height = height;
		img.width = width;
		img.data = data_;
		return img;
	}
	private static byte[] readPixesToByteArray(int[] pixes) {
		ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream();
		for (int i = 0; i < pixes.length; ++i) {
			byte[] a = new byte[3];
			a[2] = (byte)((0xff0000 & pixes[i]) >> 16);

			a[1] = (byte)((0xff00 & pixes[i]) >> 8);
			a[0] = (byte)(0xff & pixes[i]);
			try {
				byteArrayOutputStream.write(a);
			} catch (Exception e) {

			}
		}
		byte[] end = byteArrayOutputStream.toByteArray();
		return end;
	}
}
