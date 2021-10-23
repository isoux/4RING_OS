# [4RING_OS](https://www.isoux.org/blog/index.php?article2/4ring_os)

Hobby operating system for Intel processors from 386 and up. Engaged on all 4 rings with call gates, paging, segmentation and almost all hardware capabilities provided by this generation of processors.

- [Motivation](https://www.isoux.org/blog/index.php?article3/motivation)
- [Basic Idea and assumptions](https://www.isoux.org/blog/index.php?article4/basic-idea-and-assumptions)
- [Development and tools](https://www.isoux.org/blog/index.php?article5/development-and-tools)

## Release 0.00

<p style="text-align: justify;">Nothing spectacular for the zero release.</p>
<p style="text-align: justify;">What can be noticed is that the characters are printed on the monitor when the keyboard is pressed until the buffer is full. What is actually happening on the system is a "complex" int&#1077;raction between rings 1 and 0 for now, which can be seen in the code.</p>
<p style="text-align: justify;">Every interrupts made by the keyboard is captured by a procedure that is actually a task in ring one. Value is written to the queue buffer, and a request for kernel processing is sent to the core via the call gate immediately. After the core scheduler transfers that execution to the ring it requested, the task schedule procedure in that ring processes the interrupt in its domain...</p>
<p style="text-align: justify;">So each ring has its own task scheduler...</p>
<p style="text-align: justify;">To make it easier to understand the functionality of the system and for myself as well, I decided to start with release 0.00 for easier monitoring and gradual introduction to the complexity of the system. That is why ring 2 (libs) have not been launched in this release yet.</p>
<p style="text-align: justify;">The system is successfully tested on emulators as well as on real machines. It is currently compiled for i486 and 8M of RAM (for my <span style="color: #ffff00;"><a href="https://i.pinimg.com/originals/2d/24/65/2d24653bf7a545a23526c9f00fef7e11.jpg"><span style="color: #ffff00;"><em>COMPAQ AERO 486 machine</em></span></a></span>).</p>
<p style="text-align: justify;">Before I decided on GRUB loaders, with a simply written loader, I could run the system on 64-bit machines as well.</p>
<p style="text-align: justify;">However, when GRUB loaders (which are even compiled for 32-bit machines) run on 64-bit machines the system crashes. For now, I am not able to solve this problem.

In order to be able to use the BOCHS gui debbuger, I had to install an older version because the default version of BOCHS has a problem with the gui debugger on UBUNTU 20.04. Sometimes the qemu emulator works great and sometimes it crashes, which confused me, but when I checked the code on a real machine it always worked even though the qemu showed the opposite.</p>
<p>&#160;</p>
<h2>&#160;</h2>
<p>&#160;</p>
<p>&#160;</p>
