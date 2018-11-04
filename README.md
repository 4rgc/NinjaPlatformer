# NinjaPlatformer
This is a 2D platformer  project for my C++ course graduation.
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
<b>2.</b> Extract the contents of libs.zip to <i>your_solution_path/</i> <br />
<b>3.</b> Extract the contents of Debug.zip to <i>your_solution_path/Debug</i> <br />
<b>4.</b> Extract the contents of Release.zip to <i>your_solution_path/Release</i>
<br /><br />
<b> Don't forget to change VS configuration to x86, or you'll get tons of compile errors! </b>
<br /><br />
<i>Currently there are <b>5159</b> lines of code</i>
