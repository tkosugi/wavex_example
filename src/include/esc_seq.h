// for escape sequences
// Taichi Kosugi
// since Dec 11, 2017

#ifndef ESCSEQ_DEFINED
#define ESCSEQ_DEFINED

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <array>
#include <vector>
#include <sstream>


class EscSeq {
 public:
  enum class property {
    bold,
      underscore,
      blink,
      reverse,
 
      fg_black,
      fg_red,
      fg_green,
      fg_yellow,
      fg_blue,
      fg_magenta,
      fg_cyan,
      fg_white,

      bg_black,
      bg_red,
      bg_green,
      bg_yellow,
      bg_blue,
      bg_magenta,
      bg_cyan,
      bg_white
      };

  static bool is_esc_seq_used;


  static int constexpr rgb(int r, int g, int b){
    /*
    if(r < 0 || r >= 6 ||
       g < 0 || g >= 6 ||
       b < 0 || b >= 6){
      printf("invalid color\n");
      exit(1);
      }*/

    return 16 + ((r*6 + g)*6 + b);
  }


  static int constexpr grayscale(int gs){
    /*
    int const v = 0xE8 + gs;
    if(v < 0xE8 || v > 0xFF){
      printf("invalid grayscale\n");
      exit(1);
    }
    return v;
    */
    return 0xE8 + gs;
  }


  template <typename... T>
    static void eprintf(std::vector<property> const& prop, char const *fmt, T... arg){
    if(is_esc_seq_used){
      std::string str_seq("\x1b[");
      for(auto const p: prop){
	if(p == property::bold){
	  str_seq += "1";
	}else if(p == property::underscore){
	  str_seq += "4";
	}else if(p == property::blink){
	  str_seq += "5";
	}else if(p == property::reverse){
	  str_seq += "7";
	}else if(p == property::fg_black){
	  str_seq += "30";
	}else if(p == property::fg_red){
	  str_seq += "31";
	}else if(p == property::fg_green){
	  str_seq += "32";
	}else if(p == property::fg_yellow){
	  str_seq += "33";
	}else if(p == property::fg_blue){
	  str_seq += "34";
	}else if(p == property::fg_magenta){
	  str_seq += "35";
	}else if(p == property::fg_cyan){
	  str_seq += "36";
	}else if(p == property::fg_white){
	  str_seq += "37";
	}else if(p == property::bg_black){
	  str_seq += "40";
	}else if(p == property::bg_red){
	  str_seq += "41";
	}else if(p == property::bg_green){
	  str_seq += "42";
	}else if(p == property::bg_yellow){
          str_seq += "43";
        }else if(p == property::bg_blue){
          str_seq += "44";
        }else if(p == property::bg_magenta){
          str_seq += "45";
        }else if(p == property::bg_cyan){
          str_seq += "46";
        }else if(p == property::bg_white){
          str_seq += "47";
	}

	str_seq += ";";
      }

      printf((str_seq + "m" + std::string(fmt) + "\x1b[0m").c_str(), arg...);
    }else{
      printf(fmt, arg...);
    }
  }


  template <typename... T>
    static void eprintf(int color_fg, int color_bg, char const *fmt, T... arg){
    if(is_esc_seq_used){    
      std::stringstream ss;
      ss << "\x1b[38;5;" << color_fg << ";48;5;" << color_bg << "m"; 
      printf((ss.str() + std::string(fmt) + "\x1b[0m").c_str(), arg...);
    }else{
      printf(fmt, arg...);
    }
  }


  EscSeq() = delete;
};

#endif
