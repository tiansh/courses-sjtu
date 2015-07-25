#ifndef FUNCLIST_H
#define FUNCLIST_H

#include <fstream>
using namespace std;

void f_inputn     (ifstream& f);
void f_printn     (ifstream& f);
void f_inputs     (ifstream& f);
void f_prints     (ifstream& f);
void f_inputp     (ifstream& f);
void f_gotoxy     (ifstream& f);
void f_wherexy    (ifstream& f);
void f_pause      (ifstream& f);
void f_cleark     (ifstream& f);
void f_getc       (ifstream& f);
void f_upcase     (ifstream& f);
void f_putc       (ifstream& f);
void f_letn       (ifstream& f);
void f_lets       (ifstream& f);
void f_letc       (ifstream& f);
void f_add        (ifstream& f);
void f_sub        (ifstream& f);
void f_mul        (ifstream& f);
void f_divl       (ifstream& f);
void f_divh       (ifstream& f);
void f_call       (ifstream& f);
void f_gotof      (ifstream& f);
void f_exit       (ifstream& f);
void f_halt       (ifstream& f);
void f_clear      (ifstream& f);
void f_ston       (ifstream& f);
void f_ntos       (ifstream& f);
void f_ctos       (ifstream& f);
void f_cton       (ifstream& f);
void f_ntoc       (ifstream& f);
void f_stoc       (ifstream& f);
void f_lens       (ifstream& f);
void f_goto       (ifstream& f);
void f_ifs        (ifstream& f);
void f_ifn        (ifstream& f);
void f_ifc        (ifstream& f);
void f_ifp        (ifstream& f);
void f_ifpw       (ifstream& f);
void f_load       (ifstream& f);
void f_seti       (ifstream& f);
void f_getr       (ifstream& f);
void f_putr       (ifstream& f);
void f_ftor       (ifstream& f);
void f_rtof       (ifstream& f);
void f_find       (ifstream& f);
void f_del        (ifstream& f);
void f_rec        (ifstream& f);
void f_close      (ifstream& f);
void f_command    (ifstream& f);
void f_inputd     (ifstream& f);
void f_printd     (ifstream& f);
void f_inputr     (ifstream& f);
void f_printr     (ifstream& f);
void f_getdate    (ifstream& f);
void f_delst      (ifstream& f);

typedef void functype(ifstream&);
const int funcnum=55;
const struct functions {
  char n[16];
  functype* f;
} funclist[funcnum]={
{"INPUTN"     , &f_inputn     }, //  1
{"PRINTN"     , &f_printn     }, //  2
{"INPUTS"     , &f_inputs     }, //  3
{"PRINTS"     , &f_prints     }, //  4
{"INPUTP"     , &f_inputp     }, //  5
{"GOTOXY"     , &f_gotoxy     }, //  6
{"WHEREXY"    , &f_wherexy    }, //  7
{"COMMAND"    , &f_command    }, //  8
{"PAUSE"      , &f_pause      }, //  9
{"CLEARK"     , &f_cleark     }, // 10
{"GETC"       , &f_getc       }, // 11
{"PUTC"       , &f_putc       }, // 12
{"UPCASE"     , &f_upcase     }, // 13
{"LETN"       , &f_letn       }, // 14
{"LETS"       , &f_lets       }, // 15
{"LETC"       , &f_letc       }, // 16
{"ADD"        , &f_add        }, // 17
{"SUB"        , &f_sub        }, // 18
{"MUL"        , &f_mul        }, // 19
{"DIVL"       , &f_divl       }, // 20
{"DIVH"       , &f_divh       }, // 21
{"CALL"       , &f_call       }, // 22
{"GOTOF"      , &f_gotof      }, // 23
{"EXIT"       , &f_exit       }, // 24
{"HALT"       , &f_halt       }, // 25
{"CLEAR"      , &f_clear      }, // 26
{"STON"       , &f_ston       }, // 27
{"NTOS"       , &f_ntos       }, // 28
{"CTOS"       , &f_ctos       }, // 29
{"CTON"       , &f_cton       }, // 30
{"NTOC"       , &f_ntoc       }, // 31
{"STOC"       , &f_stoc       }, // 32
{"LENS"       , &f_lens       }, // 33
{"GOTO"       , &f_goto       }, // 34
{"IFS"        , &f_ifs        }, // 35
{"IFN"        , &f_ifn        }, // 36
{"IFC"        , &f_ifc        }, // 37
{"IFP"        , &f_ifp        }, // 38
{"IFPW"       , &f_ifpw       }, // 39
{"LOAD"       , &f_load       }, // 40
{"SETI"       , &f_seti       }, // 41
{"GETR"       , &f_getr       }, // 42
{"PUTR"       , &f_putr       }, // 43
{"FTOR"       , &f_ftor       }, // 44
{"RTOF"       , &f_rtof       }, // 45
{"FIND"       , &f_find       }, // 46
{"DEL"        , &f_del        }, // 47
{"REC"        , &f_rec        }, // 48
{"CLOSE"      , &f_close      }, // 49
{"INPUTD"     , &f_inputd     }, // 50
{"PRINTD"     , &f_printd     }, // 51
{"INPUTR"     , &f_inputr     }, // 52
{"PRINTR"     , &f_printr     }, // 53
{"GETDATE"    , &f_getdate    }, // 54
{"DELST"      , &f_delst      }  // 55
};

#endif
