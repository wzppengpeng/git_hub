package mr.jni;

import mr.io.*;
public class Image_jni {
	native public void predictImage(ImageContainer img, Result re);
	
	static {
		System.loadLibrary("predictImage");
	}
}
