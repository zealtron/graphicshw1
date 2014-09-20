<!doctype html>

<html lang="en">
<head>
	<meta charset="utf-8">

	<title>Assignment1</title>
	<meta name="Assignment 1" content="Image Processing">
	<meta name="James Wang" content="SitePoint">

	<link rel="stylesheet" href="css/styles.css?v=1.0">

</head>

<body>
	<header>

James Wang

jqw3ha

Compiler: g++/gnu | Environment: Linux (Ubuntu 12.04 Precise) | Platform: 64bit

Late days used: 1

	</header>
	<div id="assignment">

*   AddRandomNoise 0.5

    				![src](jqw3ha_HTML/yoda.bmp)![dest 0.5](jqw3ha_HTML/yodanoisify.bmp)
*   Brighten 2

    				![src](jqw3ha_HTML/yoda.bmp)![dest 2](jqw3ha_HTML/yodabrighten.bmp)
*   Luminance

    				![src](jqw3ha_HTML/yoda.bmp)![dest](jqw3ha_HTML/yodagray.bmp)
*   Contrast 2

    				![src](jqw3ha_HTML/shrek.bmp)![dest 2](jqw3ha_HTML/shrekcontrast.bmp)
*   Saturate 2

    				![src](jqw3ha_HTML/shrek.bmp)![dest 2](jqw3ha_HTML/shreksaturate.bmp)
*   Crop (100,100) (500,500)

    				![src](jqw3ha_HTML/yoda.bmp)![dest 400px](jqw3ha_HTML/yodacrop100to500.bmp)
*   Quantize 4

    				![src](jqw3ha_HTML/ramp.bmp)![dest 4](jqw3ha_HTML/rampquantize.bmp)
*   RandomDither 4

    				![src](jqw3ha_HTML/ramp.bmp)![dest 4](jqw3ha_HTML/ramprdither.bmp)
*   Ordered Dither 2x2 4

    				![src](jqw3ha_HTML/ramp.bmp)![dest 4](jqw3ha_HTML/rampodither.bmp)
*   FloydSteinbergDither: Not Implemented

*   Blur3x3 //Blurs image, but makes it darker

    				![src](jqw3ha_HTML/sully.bmp)![dest](jqw3ha_HTML/sullyblur.bmp)
*   Edges3x3

    				![src](jqw3ha_HTML/sully.bmp)![dest](jqw3ha_HTML/sullyedgy.bmp)
*   NearestSample: Implemented, see below

*   BilinearSample: Implemented, see below

*   GaussianSample: Not Implemented

*   ScaleNearest 1.3

    				![src](jqw3ha_HTML/stripe.bmp)![dest 1.3](jqw3ha_HTML/stripesn.bmp)
*   ScaleBilinear 1.3

    				![src](jqw3ha_HTML/stripe.bmp)![dest 1.3](jqw3ha_HTML/stripesb.bmp)
*   ScaleGaussian: Not Implemented

*   SetComposite, SetAlpha //Created a composite image of myself and a famous person!

    				![src](jqw3ha_HTML/kji.jpg)![matte](jqw3ha_HTML/matte.jpg)![overlay](jqw3ha_HTML/overlay.jpg)![dest](jqw3ha_HTML/famous.jpg)
*   FunFilter: Semi-Implemented. Swirl, doesn't quite work, but it swirls!

*   Beier-Neely Morphing: Not Implemented

*   Art Submission: Steps of each frame: saturate [-2,2] with 0.5 intervals

      				![Our lord and savior](jqw3ha_HTML/jqw3ha.art.submission1.gif)

	</div>
</body>
</html>
