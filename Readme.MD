# README #

This repository contains a way to run code inside the Wii U's web browser. It provides a means to execute arbitrary code in the Cafe OS userspace on the Espresso processor. It does **not** provide any access to the Starbuck, but now allows running code in Espresso kernel-mode. We hope to implement this in the future, but the exploit is currently limited to kernel only.

Right now, almost all firmware versions are compatible:
* Firmware versions 2.0.0 (first instalment of the Internet Browser) to 5.1.0 are supported by exploiting an use-after-free bug (CVE-2013-2842).
* Firmware version 5.3.2 is supported by exploiting a memory corruption bug (CVE-2014-1300).
* Firmware versions 5.1.1 to 5.3.1 are also supported, but currently unimplemented.
* Firmware version 5.4.0 is supported but not released yet. It have to be patched before release.
* Firmware version 5.5.0 is supported by using the same exploit than 5.4.0.

### What's inside? ###

Inside this repository, you will find a set of tools that allow you to compile your own C code and embed it inside a webpage to be executed on the Wii U. There are also a few code examples doing certain tasks on the Wii U. Most notably, there is an RPC client which allows your Wii U to execute commands sent over the network from a Python script. **Nothing** in this repository is useful to the general public. You will only find it useful if you are a C programmer and want to explore the system by yourself.

### How do I use this? ###

#### C build system ####

This repository contains a C build system, which allows you to compile your own C code and embed it inside a webpage to be executed on the Wii U. The webpage contains some Javascript code that triggers the WebKit bug and passes control to your C code. Running your own C code on the Wii U gives you full access to the SDK libraries. Any function's address can be retrieved if you know its symbol name and containing library's name (more on this below). Before using the C build system, you must have the following tools:

* Unix-like shell environment (use [Cygwin](https://cygwin.com/install.html) to get this on Windows)
* [devkitPPC](http://sourceforge.net/projects/devkitpro/) (needed to compile and link the C code), with `C:\devkitPro\devkitPPC\bin` added to `PATH`
* [Python](https://www.python.org/downloads/) 3.x (must add `C:\Python34` to `PATH`) or 2.x

#### Building ####

On Windows:

1. Install all thing you view on `C build system`.

2. Add to path (check `Adding to path`)

3. Download Libwiiu.

4. Go to libwiiu folder with: `cd c:\path\to\libwiiu`.

5. Compile using (from a Cygwin terminal)  : `python build.py your/apps/you/want/to/compile` (check `Build example`).

You will find all in www folder.

On Linux:

1. You don't have to install python/cygwin, python is pre-installed.

2. Install devkitpro by running [this script](http://sourceforge.net/projects/devkitpro/files/Automated%20Installer/devkitPPCupdate.pl/download) with the following command `sudo perl devkitPPCupdate.pl /opt/devkitpro ` (be sure that you are in the same folder than the file)

3. Open a terminal in your home directory, then do `nano .bashrc` (if you want a graphical interface, you can do this: `name_of_the_GUI_text_editor .bashrc`)

4. Copy and past each following lines at the end of the file (without any blank lines between):

export DEVKITPRO=/opt/devkitpro

export DEVKITARM=/opt/devkitpro/devkitARM

export DEVKITPPC=/opt/devkitpro/devkitPPC

export PATH=$PATH:/opt/devkitpro/devkitPPC/bin

5. Download Libwiiu.

6. Compile using : `python build.py your/apps/you/want/to/compile` (check `Build example`).

MAC OSX:

By now, I don't know how to do on MAC. Feel free to complete this part.

#### Hosting ####

On Windows:

Follow the great Bullywiiplaza's video [here](https://www.youtube.com/watch?feature=player_embedded&v=b-ztG3JmyE8).

On Linux/Mac OS X:

You can create a simple server using these commands:

`cd path/to/homebrew/`

`python -mSimpleHTTPServer 2343`

Put on your Wii U browser: http://yourinternalip:2343/

To find your IP address, do the command: `ifconfig` and look at the `wlan0/inet adr:` connection.

#### Adding to path ####

**Thank Pounou, from [gbatemp.net](https://gbatemp.net/members/pounou.353622/), for this quick tutorial**

This is for Windows user.

To edit the `PATH` environment variable:

1. Go to start menu of Windows

2. Search `environment`

3. Clic on `Modify system environment variables`

4. Clic on `environment variables`

5. In `System variable`,find one who named `Path` (or creat new one)

6. Clic on it, clic `modify`, then add the following line at the end of the existing value (do not touch the text, simply copy/past):

;C:\devkitPro\msys\bin;C:\Python27\;C:\Python27\Scripts\

#### Build Example ####
The `osscreenexamples` folder contains projects using `OSScreen` and the `loader.c` from `libwiiu`. The `examples` folder does not use `OSScreen` and uses the `loader.c` in the `projects` local directory. The `kernel` folder contains code that allows access to Espresso kernel-mod (not starbuck).
* `python build.py <project path>`
* `python build.py osscreenexamples/template`

#### Accessing the SDK libraries ####

When you're writing C code on a PC, you have the standard library, a set of useful functions that you can call without caring how they work. For writing code on the Wii U, there's something similar: the SDK libraries. The SDK libraries provide access to graphics, audio, input, filesystem, and network functions. They are accessible from any application, including the web browser, which means that code running inside it using an exploit also gets access. All SDK libraries have full symbols available for every function. This means that you can get the address of any function by its library and symbol name.

Every SDK library is an RPL file, a modification of the ELF format. For example, `gx2.rpl` is the name of the graphics library, `vpad.rpl` is the name of the Gamepad input library, and `nsysnet.rpl` is the name of the BSD sockets library. There is also a special RPL called `coreinit.rpl`, which provides direct access to many core Cafe OS services, including memory management and threading.

So how do you get the addresses of SDK functions? You could just hardcode them, obviously, but that's both lame and not portable to later exploit versions. There's a much better method available, which allows you to get symbol addresses dynamically, in the form of the `coreinit` `OSDynLoad` functions. You can access these functions by including `coreinit.h` in your C file.

There are two functions involved in getting a symbol, splitting the process into two parts. The first function is `OSDynLoad_Acquire()`, which loads the RPL you specify. `OSDynLoad_Acquire()` takes two arguments: the RPL name and a pointer to an integer handle. `OSDynLoad_Acquire()` can also be used to get a handle to a library that's already loaded. The second function is `OSDynLoad_FindExport()`, which finds a symbol given a library's location. It takes four arguments: the integer handle returned by `OSDynLoad_Acquire()`, whether the symbol is data or not (usually not), the symbol name, and a pointer to where the symbol address should be stored. Here are the function prototypes:

* `void OSDynLoad_Acquire(char \*rplname, unsigned int \*handle);`
* `void OSDynLoad_FindExport(unsigned int handle, int isdata, char \*symname, void \*address);`

Just as an example, let's say I wanted the `VPADRead()` symbol from `vpad.rpl`. If I have an integer called `handle`, I first call `OSDynLoad_Acquire("vpad.rpl", &handle);` to get the RPL's location. Next, if I have a function pointer for `VPADRead()` called `VPADRead`, I call `OSDynLoad_FindExport(handle, 0, "VPADRead", &VPADRead);` to retrive `VPADRead()`'s address. Simple. For more examples, look at `rpc.c`.

#### RPC system ####

In addition to letting you write your own C code to run on the Wii U, this repository also includes an RPC system to interactively experiment with the Wii U. It allows you to send commands over the network for your Wii U to execute. The two components of the RPC system are the server, a C program running on the Wii U listening for commands, and the client, a Python script that sends the commands.

To use the RPC system, first ensure that your PC and Wii U are connected to the same network. Once they are, find out your PC's IP address using the `ipconfig` command (Windows) or `ifconfig` command (Linux and OS X). Modify `PC_IP` in `socket.h` to be your PC's IP address (rather than `192.168.1.4`, which it currently is). Build `rpc.c` and it will go in your `htdocs`.

Next, start `rpc.py` in an interactive Python session (IDLE or IPython is a good choice). Once you've started `rpc.py`, navigate to the browser exploit you just made on your Wii U. It should appear to finish loading the page, and the UI will continue to be responsive, but web browsing will be disabled. At that point, the Python shell should say something along the lines of "Connected by" followed by your Wii U's IP. Now you can control your Wii U with these commands:

* `rpc.read32(address, num_words)` - Read *`num_words`* words starting at *`address`*, returning a list of words
* `rpc.write32(address, words)` - Write each word in the list *`words`* to memory starting at *`address`*
* `rpc.dump_mem(address, length, filename)` - Dump *`length`* bytes from memory starting at *`address`* to *`filename`*
* `symbol(rplname, symname)` - Get the symbol *`symname`* from RPL *`rplname`* and turn it into a callable Python function
* `rpc.exit()` - Exit the browser and go back to the menu

### Credits ###

* Marionumber1
* TheKit
* Hykem
* comex
* Chadderz
* Relys
* NWPlayer123
* Mathew_Wi

