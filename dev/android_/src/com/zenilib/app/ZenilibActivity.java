/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.zenilib.app;

import android.app.NativeActivity;
import android.content.res.AssetManager;
import android.os.Bundle;

public class ZenilibActivity extends NativeActivity {

  /** Native methods, implemented in jni folder */
  public static native void provideAssetManager(AssetManager assetManager);

  static {
/*    System.loadLibrary("gnustl_shared");
    System.loadLibrary("z");
    System.loadLibrary("png");
    System.loadLibrary("freetype2");
    System.loadLibrary("tinyxml");
    System.loadLibrary("3ds");
    System.loadLibrary("zeni");
    System.loadLibrary("zeni_audio");
    System.loadLibrary("zeni_core");
    System.loadLibrary("zeni_graphics");
//    System.loadLibrary("zeni_net");
    System.loadLibrary("zeni_rest");
*/    System.loadLibrary("application");
  }

  @Override
  public void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);

    // initialize the AAssetManager
    assetManager = getAssets();
    provideAssetManager(assetManager);
  }

  private static AssetManager assetManager;

}
