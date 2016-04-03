package mr.jni;
import javax.imageio.*;
import java.util.Arrays;
import java.awt.image.BufferedImage;
import java.io.*;



public class Image_jni {
	native public void initialize();
	native public void predictImage(ImageContainer img, Result re);
	public static void main(String[] argv) throws Exception {
		// String imagePath="/home/wzp/Documents/mycode/caffe_code/test_1/build/pic_002.png";
		final String imagePath = "/home/wzp/temp.jpg";
		ImageContainer img = LoadImage.loadImageToByteArray(imagePath);
		Image_jni jni = new Image_jni();
		jni.initialize();
		Result re = new Result();
		jni.predictImage(img, re);
		System.out.println(re.label + ":" + re.score);
	}
	static {
		System.loadLibrary("predictImage");
	}
}