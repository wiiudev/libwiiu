# README #

This repository contains a way to run code inside the Wii U's web browser. It provides a means to execute arbitrary code in the Cafe OS userspace on the Espresso processor. It does **not** allow running code in Espresso kernel-mode or provide any access to the Starbuck. We hope to implement this in the future, but the exploit is currently limited to userspace only. Right now, supported versions 4.0.x, 4.1.0, 5.0.0, and 5.1.0).

### What's inside? ###

Inside this repository, you will find a set of tools that allow you to compile your own C code and embed it inside a webpage to be executed on the Wii U. There are also a few code examples doing certain tasks on the Wii U. Most notably, there is an RPC client which allows your Wii U to execute commands sent over the network from a Python script. **Nothing** in this repository is useful to the general public. You will only find it useful if you are a C programmer and want to explore the system by yourself.

### How do I use this? ###

#### C build system ####

This repository contains a C build system, which allows you to compile your own C code and embed it inside a webpage to be executed on the Wii U. The webpage contains some Javascript code that triggers the WebKit bug and passes control to your C code. Running your own C code on the Wii U gives you full access to the SDK libraries. Any function's address can be retrieved if you know its symbol name and containing library's name (more on this below). Before using the C build system, you must have the following tools:

* Unix-like shell environment (use Cygwin to get this on Windows)
* devkitPPC (needed to compile and link the C code)
* Python 3.x

#### Accessing the SDK libraries ####

When you're writing C code on a PC, you have the standard library, a set of useful functions that you can call without caring how they work. For writing code on the Wii U, there's something similar: the SDK libraries. The SDK libraries provide access to graphics, audio, input, filesystem, and network functions. The SDK libraries are accessible from any application, including the web browser, which means that code running inside it using an exploit also gets access. All SDK libraries have full symbols available for every function. Aside from making reverse engineering easier, this means that you can get the address of any function by its library and symbol name.

Before using the SDK, it's important to understand the Wii U's linking model. Some of these details were provided in comex's part of the 30c3 talk, but it only touched on it. Each executable on the Wii U uses the ELF format, with slight modifications (this format is called RPX). RPX files contain a list of imports, which specify a symbol being imported from an external library along with the name of the library itself. The libraries referenced by imported symbols use the same modified ELF format, but are named RPL instead. When an RPX is loaded, the executable loader will also load its RPL dependencies.

Every SDK library is an RPL file. For example, gx2.rpl is the name of the graphics library, vpad.rpl is the name of the Gamepad input library, and nsysnet.rpl is the name of the BSD sockets library. There is also a special RPL called coreinit.rpl, which is quite interesting. coreinit provides direct access to many core Cafe OS services, including memory management and threading. coreinit was also quite useful for providing the functions we needed in our ROP chain. :)

Now that I've spent 3 paragraphs telling you how the SDK works, let's actually get around to using it. So how do you use it? The SDK libraries expose many functions, but how do you get their addresses? You could just hardcode them, obviously, but that's both lame and not portable to later exploit versions. Plus how do you find the address to hardcode in the first place? There's a much better method available, which allows you to get symbol addresses dynamically, in the form of the coreinit OSDynLoad functions. You can access these functions by including coreinit.h in your C file.

There are two functions involved in getting a symbol, splitting the process into two parts. The first function is OSDynLoad_Acquire(), which loads the RPL you specify. OSDynLoad_Acquire() takes two arguments: the RPL name as a string and a pointer to an integer. The RPL name is pretty straightforward, the pointer to an integer is where coreinit will store the library's location. OSDynLoad_Acquire() can also be used to get the location of a library that's already loaded. The second function is OSDynLoad_FindExport(), which finds a symbol given a library's location. It takes four arguments: the integer (**not** the pointer to it) used in OSDynLoad_Acquire(), whether the symbol is data or not (usually not), the symbol name as a string, and a pointer to where the symbol address should be stored. Here are the function prototypes:

* void OSDynLoad_Acquire(char \*rplname, unsigned int \*handle);
* void OSDynLoad_FindExport(unsigned int handle, int isdata, char \*symname, void \*address);

Just as an example, let's say I wanted the VPADRead() symbol from vpad.rpl. If I have an integer called "handle", I first call OSDynLoad_Acquire("vpad.rpl", &handle); to get the RPL's location. Next, if I have a function pointer for VPADRead() called "VPADRead", I call OSDynLoad_FindExport(handle, 0, "VPADRead", &VPADRead); to retrive VPADRead()'s address. Simple. For more examples, look at rpc.c.

#### RPC system ####

In addition to letting you write your own C code to run on the Wii U, this repository also includes an RPC system to interactively experiment with the Wii U. It allows you to send commands over the network for your Wii U to execute. The two components of the RPC system are the server, a C program running on the Wii U listening for commands, and the client, a Python script that sends the commands.

To use the RPC system, first ensure that your PC and Wii U are connected to the same network. Once they are, find out your PC's IP address using the "ipconfig" command (Windows) or "ifconfig" command (Linux and OS X). Modify PC_IP in socket.h to be your PC's IP address (rather than 192.168.1.4, which it currently is). Build rpc.c and it will go in your htdocs.

Next, start rpc.py in an interactive Python session (IDLE or IPython is a good choice). Once you've started rpc.py, navigate to the browser exploit you just made on your Wii U. It should appear to finish loading the page, and the UI will continue to be responsive, but web browsing will be disabled. At that point, the Python shell should say something along the lines of "Connected by" followed by your Wii U's IP. Now you can control your Wii U with these commands:

* rpc.read32(address, num_words) - Read *num_words* words starting at *address*, returning a list of words
* rpc.write32(address, words) - Write each word in the list *words* to memory starting at *address*
* symbol(rplname, symname) - Get the symbol *symname* from RPL *rplname* and turn it into a callable Python function
* rpc.exit() - Exit the browser and go back to the menu

### Credits ###

* Marionumber1 - ROP chain design/implementation
* TheKit - WebKit bug finding, ROP chain implementation
* Hykem - Finding ROP gadgets
* bubba - Testing WebKit bug candidates
* comex - Access to the coreinit and WebKit binaries
* Chadderz - Blind memory dumping on 5.0.0
* Relys - Testing exploits