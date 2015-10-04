/***************************************************/
// Animated Gif Export plugin for AviUtl           //
// by Maverick Tse, 2014                           //
/**************************************************/
DL: http://1drv.ms/1t1myhJ
Publish page: http://mavericktse.mooo.com/wordpress/archives/1826
ニコニコ: http://www.nicovideo.jp/watch/sm24236552

version 0.51 beta (2014-10-16)
*- Fixed corrupted output when width is not a multiple of 8

version 0.5 beta (2014-09-12)
*- Support saving to CJK path/file name
*- User can designate a specific color to make it transparent

version 0.1.0 beta
*- Completely fixed memory leak
*- Re-enabled color settings for ordered dithering
*- improved global color map generation

version 0.07 (Experimental release)
*- Fixed frame doubling when only one frame is selected
*- Recompiled ImageMagick to reduce dependencies and memory leak

version 0.06 (Experimental release)
*- Drastic file size reduction by providing option to drop local color tables
*- Fixed ordered-dithering options using ImageMagick API
*- Ordered-dithering of ImageMagick always use 265 colors (332 fixed color map)
*- Fixed wrong last frame index leading to adding a black frame
*- Preset buttons
*- Better error report


version 0.05 (Experimental release)
*- Added ordered dithering methods from ImageMagick (fixed to 256 color)
*- Improved color reduction at low color
*- Found potential memory leak, but no fix yet. 
Do not encode repeatedly for 5+ times.
Close and restart AviUtl to free up memory.

-------------------------------------------------------
version 0.01 (Experimental initial release)

[Introduction]
By referring to a similar plugin written by "BoiledSugar" on GitHub,
this plugin make a few steps further:

*- GUI setting dialog
*- Customizable Frame drop frequency
*- Loop count can be set (0 for infinite loop)
*- Color limit can be set (max 256 colors, min 8 colors)
*- A number of Dithering method (implemented via AForge.Net)
*- Fuzziness (-fuzz option in ImageMagick convert.exe)
*- Better reporting of encode progress

[Installation]
Copy all files to AviUtl's root folder.
If any DLL missing, install VC2013 redistributable and 
.NET framework 4.5.1 or later

[Other requirements]
The following might be required:
*- VC2013 redistributable
*- .Net Framework 4.5 or later

[Bug report]
Twitter @MaverickTse
http://mavericktse.mooo.com/wordpress/

with DETAILs please.

//*******************************************************//
The MIT License (MIT)

Copyright (c) <2014> <Maverick Tse>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
//********************************************************//
