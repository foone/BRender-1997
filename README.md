# Argonaut Blazing Render (BRender)

This is the source code to an unknown version of the BRender engine by Argonaut Software/Argonaut Games, from aproximately August 1997. 
(except for cpuid.asm. That file is from 1998, apparently)

This is intended as a basic release to let others build on, and more info on how to build and use this code will be added later.

This version had been modified to work better with the game it was intended for, and we don't have the original unmodified version of this code. There's plenty of preprocessor chunks left in the code that disable parts of the original code, or add in new elements. Those have been left in rather than backing them out, to avoid potentially breaking anything as this code doesn't neccesarily build right now. You may want to search on `#IF 0`, comments involving `<REDACTEDGAME>`, and look into the `gamerend.h` files. 

See also [the release of BRender version 1.3.2](https://github.com/foone/BRender-v1.3.2), which is believed to be untouched (or at least largely so).

## Source of this code

This was given to Foone Turing by a programmer who wishes to remain anonymous, based on a game project that had licensed BRender for use.

The name of the game was listed in this archive in several comments and filenames, it's been replaced by REDACTED to not reveal what game it was, as approval to open source that game hasn't been given at this time. 

## Approval to open source this software

Approval to open source this was given in an email from Jez San, former CEO of Argonaut. 

## Credits

| Role          | Name           | 
| ------------- | -------------  |
| Programming   | `Sam Littlewood` `Dan Piponi` `Simon Everett` `Philip Pratt` |
| Ports         | `Simon Everett` `Philip Pratt` `Patrick Buckland` `Zareh Johannes`  `Anthony Savva` `Stainless Software Ltd`    |
| Testing       | `Philip Pratt`  |
| Project Management | `Stefano Zammattio` |
| Technical Reference Manual | `Crosbie Fitch` `Philip Pratt` |
| Tutorial & Installation Guides | `Robbie McQuaid` | 
| Marketing | `Paul Ayscough` `Mitra Faulkner` |
| Sales | `Edwin Masterson` |
| Artwork| `Michel Bowes` |
| Technical Support | `Neela Dass` `Tony Roberts` `John Gay` `Jon Wolff` |
| With Thanks to | `Pete Warnes` `Mike Day` `Tim Gummer` `Vinay Gupta` `Marcia Petterson` `Jez San` | 

(From the 1996 Technical Reference Manual. More names will be added if they are uncovered)

The open sourcing of this version was done by Scott Hanselman and Foone Turing, with help from an anonymous contributor. 

## License

This is released under the MIT license.


Copyright (c) 1998 Argonaut Software Limited 

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.