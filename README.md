# NinjaPlatformer
This is a 2D platformer  project for my STEP academy C++ course graduation. (works in progress)
<br /><br />
I was using Visual Studio 2017 Community, so this project is set to v141 Build tools, platform 10.0.10586.0.<br />
All code inside Angine and NinjaPlatformer I wrote *myself*.<br />
To get <b>best</b> performance launch game in Release configuraion.
<br /><br />
This has been created with SDL2, OpenGL and Box2D libraries.
<br /><br />
<b/>/Angine</b> - the game engine folder.<br />
Here is all the interesting stuff about "inside working".
<br /><br />
<b>/libs</b> - dependencies folder. <br />
The solution is set to get all the includes from "$(SolutionDir)/libs/include"<br /> and the libraries from "$(SolutionDir)/libs/lib"
<br /><br />
<b>/NinjaPlatformer</b> - the actual game folder. <br />
All the game physics and mechanics are in here.
<br /><br />
<b>Installation</b><br />
<b>1.</b> Put everything from the cloned repo into one folder <br />
<b>2.</b> Extract and merge the libs.zip to <i>your_solution_path/</i> <br />
<b>3.</b> Extract contents of bin_Debug.zip into <i>your_solution_path/Debug</i> <br />
<b>4.</b> Extract contents of bin_Release.zip into <i>your_solution_path/Release</i>
<br /><br />
<b>If you have VS2015:</b> <br />
<b>1.</b> Change your build tools to v140: <br >
&nbsp;&nbsp;&nbsp;&nbsp;*Solution Explorer window->Angine->Right-click mouse->Properties->*<br />
&nbsp;&nbsp;&nbsp;&nbsp;*->Platform toolset->Visual Studio 2015 v140->OK*<br/>
&nbsp;&nbsp;&nbsp;&nbsp;*Solution Explorer window->NinjaPlatformer->Right-click mouse->Properties->*<br />
&nbsp;&nbsp;&nbsp;&nbsp;*Platform toolset->Visual Studio 2015 v140->OK* <br />
<b>2.</b> Change Target platform to lastest available (usually 10.0.10240.0): <br />
&nbsp;&nbsp;&nbsp;&nbsp;*Solution Explorer window->Angine->Right-click mouse->Properties->*<br />
&nbsp;&nbsp;&nbsp;&nbsp;*->Target Platform Version->10.0.10240.0->OK*<br />
&nbsp;&nbsp;&nbsp;&nbsp;*Solution Explorer window->NinjaPlatformer->Right-click mouse->Properties->*<br />
&nbsp;&nbsp;&nbsp;&nbsp;*->Target Platform Version->10.0.10240.0->OK*
<br /> <br />
Do these steps for both Debug and Release configurations!!!
<br /><br />
<b> Don't forget to change VS configuration to x86, or you'll get tons of compile errors! </b>
<br /><br />
<i>Currently there are <b>5159</b> lines of code</i>
