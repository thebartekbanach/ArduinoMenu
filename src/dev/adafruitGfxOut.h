/********************
Sept. 2014 Rui Azevedo - ruihfazevedo(@rrob@)gmail.com
creative commons license 3.0: Attribution-ShareAlike CC BY-SA
This software is furnished "as is", without technical support, and with no
warranty, express or implied, as to its usefulness for any purpose.

Thread Safe: No
Extensible: Yes

Use graphics screens (adafruit library based) as menu output
www.r-site.net
***/
#ifndef RSITE_ARDUINOP_MENU_GFX
	#define RSITE_ARDUINOP_MENU_GFX
	#include <Adafruit_GFX.h>
	#include "../menu.h"

namespace Menu {

		#define RGB565(r,g,b) ((((r>>3)<<11) | ((g>>2)<<5) | (b>>3)))

		// Color definitions RGB565
		#define BLACK 0x0000
		#define BLUE 0x001F
		#define RED 0xF800
		#define GREEN 0x07E0
		#define GRAY RGB565(128,128,128)
		#define SILVER RGB565(200,200,200)
		#define CYAN 0x07FF
		#define MAGENTA 0xF81F
		#define YELLOW 0xFFE0
		#define WHITE 0xFFFF

	  class menuGFX:public gfxOut {
	    public:
				Adafruit_GFX& gfx;
				const uint16_t (&colors)[11][2][2];
		    menuGFX(Adafruit_GFX& gfx,const uint16_t (&c)[11][2][2],int resX=6,int resY=9)
			  	:gfxOut(gfx.width()/resX,gfx.height()/resY,resX,resY),colors(c),gfx(gfx) {}

				//virtual colorFmt getColor(colorDefs,status,colorPair)=0;
				//TODO: Ok, virtual cant return template parameter type... now what?
				void setColor(colorDefs c,status s) override {
					const uint16_t* bf=colors[c][s];
					gfx.setTextColor(memWord(bf[foreground]));//,memWord(bf[background]));
				}

				void invColor(colorDefs c,status s) override {
					const uint16_t* bf=colors[c][s];
					gfx.setTextColor(memWord(bf[background]));
				}

		    void clearLine(int ln) override {
		    	gfx.fillRect(0,ln*resY,resX*maxX,resY,colors[optionColor][enabledStatus][background]);
		    	setCursor(0,ln);
		    }
		    void clear() override {
		    	gfx.fillRect(0,0,resX*maxX,resY*maxY,colors[optionColor][enabledStatus][background]);
		    	setCursor(0,0);
		    }
		    void setCursor(int x,int y) override {gfx.setCursor(x*resX,y*resY);}
		    size_t write(uint8_t ch) override {return gfx.write(ch);}
				void printMenu(navNode &nav) override {
					idx_t ot=top;
          while(nav.sel>=top+maxY) top++;
          while(nav.sel<top) top--;
          if (drawn!=nav.target) {
						clear();
						drawn=nav.target;
					}
					if (top!=lastTop||lastSel!=nav.sel||top!=ot||drawn!=nav.target||nav.target->changed(nav,*this)) {
						for(idx_t i=0;i<maxY;i++) {
              if (i+top>=nav.sz()) break;
              clearLine(i);
              setCursor(0,i);
              prompt& p=nav[i+top];
							setColor(cursorColor,p.enabled);
              write(i+top==nav.sel?options.selectedCursor:' ');
							setColor(nav.sel==i+top?optionColorHi:optionColor,p.enabled);
              p.printTo(i,nav,*this);
            }
            lastTop=top;
            lastSel=nav.sel;
						//gfx.display();//this is device specific!
          }
				}
	  };

}; //namespace menuGFX
#endif
