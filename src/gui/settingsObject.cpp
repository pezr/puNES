/*
 * settingsObject.cpp
 *
 *  Created on: 24/ott/2014
 *      Author: fhorse
 */

#include <QtCore/QStringList>
#include <QtCore/QTextStream>
#include "settingsObject.moc"
#include "clock.h"
#include "save_slot.h"
#include "conf.h"
#include "emu.h"
#include "jstick.h"
#include "gui.h"
#if defined (__linux__)
#define XK_MISCELLANY
#include <X11/keysymdef.h>
#endif

extern _emu_settings s;

static const struct _kvSpecials {
	quint32 native;
	Qt::KeyboardModifiers modifiers;
	DBWORD key;
	char name[20];
	DBWORD value;
} kvSpecials[] = {
	{ 0,            Qt::KeypadModifier,  Qt::Key_0,        "Numpad0",    257 },
	{ 0,            Qt::KeypadModifier,  Qt::Key_Insert,   "NumPad0",    257 },
	{ 0,            Qt::KeypadModifier,  Qt::Key_1,        "NumPad1",    258 },
	{ 0,            Qt::KeypadModifier,  Qt::Key_End,      "NumPad1",    258 },
	{ 0,            Qt::KeypadModifier,  Qt::Key_2,        "NumPad2",    259 },
	{ 0,            Qt::KeypadModifier,  Qt::Key_Down,     "NumPad2",    259 },
	{ 0,            Qt::KeypadModifier,  Qt::Key_3,        "NumPad3",    260 },
	{ 0,            Qt::KeypadModifier,  Qt::Key_PageDown, "NumPad3",    260 },
	{ 0,            Qt::KeypadModifier,  Qt::Key_4,        "NumPad4",    261 },
	{ 0,            Qt::KeypadModifier,  Qt::Key_Left,     "NumPad4",    261 },
	{ 0,            Qt::KeypadModifier,  Qt::Key_5,        "NumPad5",    262 },
	{ 0,            Qt::KeypadModifier,  Qt::Key_Clear,    "NumPad5",    262 },
	{ 0,            Qt::KeypadModifier,  Qt::Key_6,        "NumPad6",    263 },
	{ 0,            Qt::KeypadModifier,  Qt::Key_Right,    "NumPad6",    263 },
	{ 0,            Qt::KeypadModifier,  Qt::Key_7,        "NumPad7",    264 },
	{ 0,            Qt::KeypadModifier,  Qt::Key_Home,     "NumPad7",    264 },
	{ 0,            Qt::KeypadModifier,  Qt::Key_8,        "NumPad8",    265 },
	{ 0,            Qt::KeypadModifier,  Qt::Key_Up,       "NumPad8",    265 },
	{ 0,            Qt::KeypadModifier,  Qt::Key_9,        "NumPad9",    266 },
	{ 0,            Qt::KeypadModifier,  Qt::Key_PageUp,   "NumPad9",    266 },
	{ 0,            Qt::KeypadModifier,  Qt::Key_Asterisk, "NPMultiply", 267 },
	{ 0,            Qt::KeypadModifier,  Qt::Key_Plus,     "NPAdd",      268 },
	{ 0,            Qt::KeypadModifier,  Qt::Key_Delete,   "NPDecimal",  269 },
	{ 0,            Qt::KeypadModifier,  Qt::Key_Minus,    "NPSubtract", 270 },
	{ 0,            Qt::KeypadModifier,  Qt::Key_Period,   "NPDecimal",  271 },
	{ 0,            Qt::KeypadModifier,  Qt::Key_Slash,    "NPDivide",   272 },
#if defined (__WIN32__)
	{ VK_LMENU,     0,                   Qt::Key_Alt,      "Alt",        273 },
	{ VK_RMENU,     0,                   Qt::Key_Alt,      "AltGr",      274 },
	{ 0,            Qt::NoModifier,      Qt::Key_Meta,     "Meta",       275 },
	{ VK_LSHIFT,    0,                   Qt::Key_Shift,    "LShift",     276 },
	{ VK_RSHIFT,    0,                   Qt::Key_Shift,    "RShift",     277 },
	{ VK_LCONTROL,  0,                   Qt::Key_Control,  "LCtrl",      278 },
	{ VK_RCONTROL,  0,                   Qt::Key_Control,  "RCtrl",      279 },
#elif defined (__linux__)
	/*
	{ 0,            Qt::AltModifier,     Qt::Key_Alt,      "Alt",        273 },
	{ 0,            Qt::NoModifier,      Qt::Key_AltGr,    "AltGr",      274 },
	{ 0,            Qt::MetaModifier,    Qt::Key_Meta,     "Meta",       275 },
	{ 0,            Qt::ShiftModifier,   Qt::Key_Shift,    "LShift",     276 },
	{ 0,            Qt::ShiftModifier,   Qt::Key_Shift,    "RShift",     277 },
	{ 0,            Qt::ControlModifier, Qt::Key_Control,  "LCtrl",      278 },
	{ 0,            Qt::ControlModifier, Qt::Key_Control,  "RCtrl",      279 },
	*/
	{ 0,            Qt::NoModifier,      Qt::Key_Alt,      "Alt",        273 },
	{ 0,            Qt::NoModifier,      Qt::Key_AltGr,    "AltGr",      274 },
	{ 0,            Qt::NoModifier,      Qt::Key_Meta,     "Meta",       275 },
	{ XK_Shift_L,   0,                   Qt::Key_Shift,    "LShift",     276 },
	{ XK_Shift_R,   0,                   Qt::Key_Shift,    "RShift",     277 },
	{ XK_Control_L, 0,                   Qt::Key_Control,  "LCtrl",      278 },
	{ XK_Control_R, 0,                   Qt::Key_Control,  "RCtrl",      279 },
#endif
};
/*
static const struct _kvTable {
	DBWORD key;
	char name[40];
} kvTable[] = {
	{ Qt::Key_Escape,                 "Esc"        },
	{ Qt::Key_Tab,                    "Tab"        },
	{ Qt::Key_Backtab,                "Backtab"    },
	{ Qt::Key_Backspace,              "Backspace"  },
	{ Qt::Key_Return,                 "Return"     },
	{ Qt::Key_Enter,                  "Enter"      },
	{ Qt::Key_Insert,                 "Ins"        },
	{ Qt::Key_Delete,                 "Del"        },
	{ Qt::Key_Pause,                  "Pause"      },
	{ Qt::Key_Print,                  "Print"      },
	{ Qt::Key_SysReq,                 "SysReq"     },
	{ Qt::Key_Clear,                  "Clear"      },
	{ Qt::Key_Home,                   "Home"       },
	{ Qt::Key_End,                    "End"        },
	{ Qt::Key_Left,                   "Left"       },
	{ Qt::Key_Up,                     "Up"         },
	{ Qt::Key_Right,                  "Right"      },
	{ Qt::Key_Down,                   "Down"       },
	{ Qt::Key_PageUp,                 "PageUp"     },
	{ Qt::Key_PageDown,               "PageDown"   },
	{ Qt::Key_Shift,                  "Shift"      },
	{ Qt::Key_Control,                "Control"    },
	{ Qt::Key_Meta,                   "Meta"       },
	{ Qt::Key_Alt,                    "Alt"        },
	{ Qt::Key_AltGr,                  "AltGr"      },
	{ Qt::Key_CapsLock,               "CapsLock"   },
	{ Qt::Key_NumLock,                "NumLock"    },
	{ Qt::Key_ScrollLock,             "ScrlLock"   },
	{ Qt::Key_F1,                     "F1"         },
	{ Qt::Key_F2,                     "F2"         },
	{ Qt::Key_F3,                     "F3"         },
	{ Qt::Key_F4,                     "F4"         },
	{ Qt::Key_F5,                     "F5"         },
	{ Qt::Key_F6,                     "F6"         },
	{ Qt::Key_F7,                     "F7"         },
	{ Qt::Key_F8,                     "F8"         },
	{ Qt::Key_F9,                     "F9"         },
	{ Qt::Key_F10,                    "F10"        },
	{ Qt::Key_F11,                    "F11"        },
	{ Qt::Key_F12,                    "F12"        },
	{ Qt::Key_F13,                    "F13"        },
	{ Qt::Key_F14,                    "F14"        },
	{ Qt::Key_F15,                    "F15"        },
	{ Qt::Key_F16,                    "F16"        },
	{ Qt::Key_F17,                    "F17"        },
	{ Qt::Key_F18,                    "F18"        },
	{ Qt::Key_F19,                    "F19"        },
	{ Qt::Key_F20,                    "F20"        },
	{ Qt::Key_F21,                    "F21"        },
	{ Qt::Key_F22,                    "F22"        },
	{ Qt::Key_F23,                    "F23"        },
	{ Qt::Key_F24,                    "F24"        },
	{ Qt::Key_F25,                    "F25"        },
	{ Qt::Key_F26,                    "F26"        },
	{ Qt::Key_F27,                    "F27"        },
	{ Qt::Key_F28,                    "F28"        },
	{ Qt::Key_F29,                    "F29"        },
	{ Qt::Key_F30,                    "F30"        },
	{ Qt::Key_F31,                    "F31"        },
	{ Qt::Key_F32,                    "F32"        },
	{ Qt::Key_F33,                    "F33"        },
	{ Qt::Key_F34,                    "F34"        },
	{ Qt::Key_F35,                    "F35"        },
	{ Qt::Key_Super_L,                "SuperL"     },
	{ Qt::Key_Super_R,                "SuperR"     },
	{ Qt::Key_Menu,                   "Menu"       },
	{ Qt::Key_Hyper_L,                "HyperL"     },
	{ Qt::Key_Hyper_R,                "HyperR"     },
	{ Qt::Key_Help,                   "Help"       },
	{ Qt::Key_Direction_L,            "DirectionL" },
	{ Qt::Key_Direction_R,            "DirectionR" },
	{ Qt::Key_Space,                  "Space"      },
	{ Qt::Key_Any,                    "Any"        },
	{ Qt::Key_Exclam,                 "Exclam"     },
	{ Qt::Key_QuoteDbl,               "QuoteDbl"   },
	{ Qt::Key_NumberSign,             "NumberSign" },
	{ Qt::Key_Dollar,                 "Dollar"     },
	{ Qt::Key_Percent,                "Percent"    },
	{ Qt::Key_Ampersand,              "Ampersand"  },
	{ Qt::Key_Apostrophe,             "Apostrophe" },
	{ Qt::Key_ParenLeft,              "ParenLeft"  },
	{ Qt::Key_ParenRight,             "ParenRight" },
	{ Qt::Key_Asterisk,               "Asterisk"   },
	{ Qt::Key_Plus,                   "Plus"       },
	{ Qt::Key_Comma,                  "Comma"      },
	{ Qt::Key_Minus,                  "Minus"      },
	{ Qt::Key_Period,                 "Period"     },
	{ Qt::Key_Slash,                  "Slash"      },
//	{ Qt::Key_0,                      "0"          },
//	{ Qt::Key_1,                      "1"          },
//	{ Qt::Key_2,                      "2"          },
//	{ Qt::Key_3,                      "3"          },
//	{ Qt::Key_4,                      "4"          },
//	{ Qt::Key_5,                      "5"          },
//	{ Qt::Key_6,                      "6"          },
//	{ Qt::Key_7,                      "7"          },
//	{ Qt::Key_8,                      "8"          },
//	{ Qt::Key_9,                      "9"          },
	{ Qt::Key_Colon,                  "Colon"      },
	{ Qt::Key_Semicolon,              "Semicolon"  },
	{ Qt::Key_Less,                   "Less"       },
	{ Qt::Key_Equal,                  "Equal"      },
	{ Qt::Key_Greater,                "Greater"    },
	{ Qt::Key_Question,               "Question"   },
	{ Qt::Key_At,                     "At"         },
//	{ Qt::Key_A,                      "A"          },
//	{ Qt::Key_B,                      "B"          },
//	{ Qt::Key_C,                      "C"          },
//	{ Qt::Key_D,                      "D"          },
//	{ Qt::Key_E,                      "E"          },
//	{ Qt::Key_F,                      "F"          },
//	{ Qt::Key_G,                      "G"          },
//	{ Qt::Key_H,                      "H"          },
//	{ Qt::Key_I,                      "I"          },
//	{ Qt::Key_J,                      "J"          },
//	{ Qt::Key_K,                      "K"          },
//	{ Qt::Key_L,                      "L"          },
//	{ Qt::Key_M,                      "M"          },
//	{ Qt::Key_N,                      "N"          },
//	{ Qt::Key_O,                      "O"          },
//	{ Qt::Key_P,                      "P"          },
//	{ Qt::Key_Q,                      "Q"          },
//	{ Qt::Key_R,                      "R"          },
//	{ Qt::Key_S,                      "S"          },
//	{ Qt::Key_T,                      "T"          },
//	{ Qt::Key_U,                      "U"          },
//	{ Qt::Key_V,                      "V"          },
//	{ Qt::Key_W,                      "W"          },
//	{ Qt::Key_X,                      "X"          },
//	{ Qt::Key_Y,                      "Y"          },
//	{ Qt::Key_Z,                      "Z"          },
	{ Qt::Key_BracketLeft,            "BrckLeft"   },
	{ Qt::Key_Backslash,              "Backslash"  },
	{ Qt::Key_BracketRight,           "BrckRight"  },
	{ Qt::Key_AsciiCircum,            "AsciiCrcm"  },
	{ Qt::Key_Underscore,             "Underscore" },
	{ Qt::Key_QuoteLeft,              "QuoteLeft"  },
	{ Qt::Key_BraceLeft,              "BraceLeft"  },
	{ Qt::Key_Bar,                    "Bar"        },
	{ Qt::Key_BraceRight,             "BraceRight" },
	{ Qt::Key_AsciiTilde,             "AsciiTilde" },
	{ Qt::Key_nobreakspace,           "NoBrkSpace" },
	{ Qt::Key_exclamdown,             "ExclamDown" },
	{ Qt::Key_cent,                   "Cent"       },
	{ Qt::Key_sterling,               "Sterling"   },
	{ Qt::Key_currency,               "Currency"   },
	{ Qt::Key_yen,                    "Yen"        },
	{ Qt::Key_brokenbar,              "Brokenbar"  },
	{ Qt::Key_section,                "Section"    },
	{ Qt::Key_diaeresis,              "Diaeresis"  },
	{ Qt::Key_copyright,              "Copyright"  },
	{ Qt::Key_ordfeminine,            "OrdFem"     },
	{ Qt::Key_guillemotleft,          "GlmtLeft"   },
	{ Qt::Key_notsign,                "NotSign"    },
	{ Qt::Key_hyphen,                 "Hyphen"     },
	{ Qt::Key_registered,             "Registered" },
	{ Qt::Key_macron,                 "Macron"     },
	{ Qt::Key_degree,                 "Degree"     },
	{ Qt::Key_plusminus,              "Plusminus"  },
	{ Qt::Key_twosuperior,            "2Superior"  },
	{ Qt::Key_threesuperior,          "3Superior"  },
	{ Qt::Key_acute,                  "Acute"      },
	{ Qt::Key_mu,                     "Mu"         },
	{ Qt::Key_paragraph,              "Paragraph"  },
	{ Qt::Key_periodcentered,         "PrCentered" },
	{ Qt::Key_cedilla,                "Cedilla"    },
	{ Qt::Key_onesuperior,            "1Superior"  },
	{ Qt::Key_masculine,              "Masculine"  },
	{ Qt::Key_guillemotright,         "GlmtRight"  },
	{ Qt::Key_onequarter,             "OneQuarter" },
	{ Qt::Key_onehalf,                "OneHalf"    },
	{ Qt::Key_threequarters,          "3Quarters"  },
	{ Qt::Key_questiondown,           "QuestionDw" },
	{ Qt::Key_Agrave,                 "Agrave"     },
	{ Qt::Key_Aacute,                 "Aacute"     },
	{ Qt::Key_Acircumflex,            "Acircumflx" },
	{ Qt::Key_Atilde,                 "Atilde"     },
	{ Qt::Key_Adiaeresis,             "Adiaeresis" },
	{ Qt::Key_Aring,                  "Aring"      },
	{ Qt::Key_AE,                     "AE"         },
	{ Qt::Key_Ccedilla,               "Ccedilla"   },
	{ Qt::Key_Egrave,                 "Egrave"     },
	{ Qt::Key_Eacute,                 "Eacute"     },
	{ Qt::Key_Ecircumflex,            "Ecircumflx" },
	{ Qt::Key_Ediaeresis,             "Ediaeresis" },
	{ Qt::Key_Igrave,                 "Igrave"     },
	{ Qt::Key_Iacute,                 "Iacute"     },
	{ Qt::Key_Icircumflex,            "Icircumflx" },
	{ Qt::Key_Idiaeresis,             "Idiaeresis" },
	{ Qt::Key_ETH,                    "ETH"        },
	{ Qt::Key_Ntilde,                 "Ntilde"     },
	{ Qt::Key_Ograve,                 "Ograve"     },
	{ Qt::Key_Oacute,                 "Oacute"     },
	{ Qt::Key_Ocircumflex,            "Ocircumflx" },
	{ Qt::Key_Otilde,                 "Otilde"     },
	{ Qt::Key_Odiaeresis,             "Odiaeresis" },
	{ Qt::Key_multiply,               "Multiply"   },
	{ Qt::Key_Ooblique,               "Ooblique"   },
	{ Qt::Key_Ugrave,                 "Ugrave"     },
	{ Qt::Key_Uacute,                 "Uacute"     },
	{ Qt::Key_Ucircumflex,            "Ucircumflx" },
	{ Qt::Key_Udiaeresis,             "Udiaeresis" },
	{ Qt::Key_Yacute,                 "Yacute"     },
	{ Qt::Key_THORN,                  "THORN"      },
	{ Qt::Key_ssharp,                 "Ssharp"     },
	{ Qt::Key_division,               "Division"   },
	{ Qt::Key_ydiaeresis,             "ydiaeresis" },
	{ Qt::Key_Multi_key,              "MultiKey"   },
	{ Qt::Key_Codeinput,              "Codeinput"  },
	{ Qt::Key_SingleCandidate,        "SCandidate" },
	{ Qt::Key_MultipleCandidate,      "MCandidate" },
	{ Qt::Key_PreviousCandidate,      "PCandidate" },
	{ Qt::Key_Mode_switch,            "ModeSwitch" },
	{ Qt::Key_Kanji,                  "Kanji"      },
	{ Qt::Key_Muhenkan,               "Muhenkan"   },
	{ Qt::Key_Henkan,                 "Henkan"     },
	{ Qt::Key_Romaji,                 "Romaji"     },
	{ Qt::Key_Hiragana,               "Hiragana"   },
	{ Qt::Key_Katakana,               "Katakana"   },
	{ Qt::Key_Hiragana_Katakana,      "HKatakana"  },
	{ Qt::Key_Zenkaku,                "Zenkaku"    },
	{ Qt::Key_Hankaku,                "Hankaku"    },
	{ Qt::Key_Zenkaku_Hankaku,        "ZHankaku"   },
	{ Qt::Key_Touroku,                "Touroku"    },
	{ Qt::Key_Massyo,                 "Massyo"     },
	{ Qt::Key_Kana_Lock,              "KanaLock"   },
	{ Qt::Key_Kana_Shift,             "KanaShift"  },
	{ Qt::Key_Eisu_Shift,             "EisuShift"  },
	{ Qt::Key_Eisu_toggle,            "EisuToggle" },
	{ Qt::Key_Hangul,                 "Hangul"     },
	{ Qt::Key_Hangul_Start,           "HngStart"   },
	{ Qt::Key_Hangul_End,             "HngEnd"     },
	{ Qt::Key_Hangul_Hanja,           "HngHanja"   },
	{ Qt::Key_Hangul_Jamo,            "HngJamo"    },
	{ Qt::Key_Hangul_Romaja,          "HngRomaja"  },
	{ Qt::Key_Hangul_Jeonja,          "HngJeonja"  },
	{ Qt::Key_Hangul_Banja,           "HngBanja"   },
	{ Qt::Key_Hangul_PreHanja,        "HngPrHanja" },
	{ Qt::Key_Hangul_PostHanja,       "HngPsHanja" },
	{ Qt::Key_Hangul_Special,         "HngSpecial" },
	{ Qt::Key_Dead_Grave,             "DGrave"     },
	{ Qt::Key_Dead_Acute,             "DAcute"     },
	{ Qt::Key_Dead_Circumflex,        "DCircumflx" },
	{ Qt::Key_Dead_Tilde,             "DTilde"     },
	{ Qt::Key_Dead_Macron,            "DMacron"    },
	{ Qt::Key_Dead_Breve,             "DBreve"     },
	{ Qt::Key_Dead_Abovedot,          "DAbovedot"  },
	{ Qt::Key_Dead_Diaeresis,         "DDiaeresis" },
	{ Qt::Key_Dead_Abovering,         "DAbovering" },
	{ Qt::Key_Dead_Doubleacute,       "DDblacute"  },
	{ Qt::Key_Dead_Caron,             "DCaron"     },
	{ Qt::Key_Dead_Cedilla,           "DCedilla"   },
	{ Qt::Key_Dead_Ogonek,            "DOgonek"    },
	{ Qt::Key_Dead_Iota,              "DIota"      },
	{ Qt::Key_Dead_Voiced_Sound,      "DVoiced"    },
	{ Qt::Key_Dead_Semivoiced_Sound,  "DSemivoice" },
	{ Qt::Key_Dead_Belowdot,          "DBelowdot"  },
	{ Qt::Key_Dead_Hook,              "DHook"      },
	{ Qt::Key_Dead_Hor,               "DHorn"      },
	{ Qt::Key_Back,                   "Back"       },
	{ Qt::Key_Forward,                "Forward"    },
	{ Qt::Key_Stop,                   "Stop"       },
	{ Qt::Key_Refresh,                "Refresh"    },
	{ Qt::Key_VolumeDown,             "VolumeDown" },
	{ Qt::Key_VolumeMute,             "VolumeMute" },
	{ Qt::Key_VolumeUp,               "VolumeUp"   },
	{ Qt::Key_BassBoost,              "BassBoost"  },
	{ Qt::Key_BassUp,                 "BassUp"     },
	{ Qt::Key_BassDown,               "BassDown"   },
	{ Qt::Key_TrebleUp,               "TrebleUp"   },
	{ Qt::Key_TrebleDown,             "TrebleDown" },
	{ Qt::Key_MediaPlay,              "MediaPlay"  },
	{ Qt::Key_MediaStop,              "MediaStop"  },
	{ Qt::Key_MediaPrevious,          "MediaPrvs"  },
	{ Qt::Key_MediaNext,              "MediaNext"  },
	{ Qt::Key_MediaRecord,            "MediaRcd"   },
	{ Qt::Key_MediaPause,             "MediaPause" },
	{ Qt::Key_MediaTogglePlayPause,   "MPlayPause" },
	{ Qt::Key_HomePage,               "HomePage"   },
	{ Qt::Key_Favorites,              "Favorites"  },
	{ Qt::Key_Search,                 "Search"     },
	{ Qt::Key_Standby,                "Standby"    },
	{ Qt::Key_OpenUrl,                "OpenUrl"    },
	{ Qt::Key_LaunchMail,             "LaunchMail" },
	{ Qt::Key_LaunchMedia,            "LaunchMdia" },
	{ Qt::Key_Launch0,                "Launch0"    },
	{ Qt::Key_Launch1,                "Launch1"    },
	{ Qt::Key_Launch2,                "Launch2"    },
	{ Qt::Key_Launch3,                "Launch3"    },
	{ Qt::Key_Launch4,                "Launch4"    },
	{ Qt::Key_Launch5,                "Launch5"    },
	{ Qt::Key_Launch6,                "Launch6"    },
	{ Qt::Key_Launch7,                "Launch7"    },
	{ Qt::Key_Launch8,                "Launch8"    },
	{ Qt::Key_Launch9,                "Launch9"    },
	{ Qt::Key_LaunchA,                "LaunchA"    },
	{ Qt::Key_LaunchB,                "LaunchB"    },
	{ Qt::Key_LaunchC,                "LaunchC"    },
	{ Qt::Key_LaunchD,                "LaunchD"    },
	{ Qt::Key_LaunchE,                "LaunchE"    },
	{ Qt::Key_LaunchF,                "LaunchF"    },
	{ Qt::Key_LaunchG,                "LaunchG"    },
	{ Qt::Key_LaunchH,                "LaunchH"    },
	{ Qt::Key_MonBrightnessUp,        "MonBrUp"    },
	{ Qt::Key_MonBrightnessDown,      "MonBrDown"  },
	{ Qt::Key_KeyboardLightOnOff,     "KbdLOnOff"  },
	{ Qt::Key_KeyboardBrightnessUp,   "KbdBrUp"    },
	{ Qt::Key_KeyboardBrightnessDown, "KbdBrDown"  },
	{ Qt::Key_PowerOff,               "PowerOff"   },
	{ Qt::Key_WakeUp,                 "WakeUp"     },
	{ Qt::Key_Eject,                  "Eject"      },
	{ Qt::Key_ScreenSaver,            "ScrnSaver"  },
	{ Qt::Key_WWW,                    "WWW"        },
	{ Qt::Key_Memo,                   "Memo"       },
	{ Qt::Key_LightBulb,              "LightBulb"  },
	{ Qt::Key_Shop,                   "Shop"       },
	{ Qt::Key_History,                "History"    },
	{ Qt::Key_AddFavorite,            "AddFvrite"  },
	{ Qt::Key_HotLinks,               "HotLinks"   },
	{ Qt::Key_BrightnessAdjust,       "BrghAdjust" },
	{ Qt::Key_Finance,                "Finance"    },
	{ Qt::Key_Community,              "Community"  },
	{ Qt::Key_AudioRewind,            "AudioRwnd"  },
	{ Qt::Key_BackForward,            "BackFrwrd"  },
	{ Qt::Key_ApplicationLeft,        "AppLeft"    },
	{ Qt::Key_ApplicationRight,       "AppRight"   },
	{ Qt::Key_Book,                   "Book"       },
	{ Qt::Key_CD,                     "CD"         },
	{ Qt::Key_Calculator,             "Calculator" },
	{ Qt::Key_ToDoList,               "ToDoList"   },
	{ Qt::Key_ClearGrab,              "ClearGrab"  },
	{ Qt::Key_Close,                  "Close"      },
	{ Qt::Key_Copy,                   "Copy"       },
	{ Qt::Key_Cut,                    "Cut"        },
	{ Qt::Key_Display,                "Display"    },
	{ Qt::Key_DOS,                    "DOS"        },
	{ Qt::Key_Documents,              "Documents"  },
	{ Qt::Key_Excel,                  "Excel"      },
	{ Qt::Key_Explorer,               "Explorer"   },
	{ Qt::Key_Game,                   "Game"       },
	{ Qt::Key_Go,                     "Go"         },
	{ Qt::Key_iTouch,                 "iTouch"     },
	{ Qt::Key_LogOff,                 "LogOff"     },
	{ Qt::Key_Market,                 "Market"     },
	{ Qt::Key_Meeting,                "Meeting"    },
	{ Qt::Key_MenuKB,                 "MenuKB"     },
	{ Qt::Key_MenuPB,                 "MenuPB"     },
	{ Qt::Key_MySites,                "MySites"    },
	{ Qt::Key_News,                   "News"       },
	{ Qt::Key_OfficeHome,             "OfficeHome" },
	{ Qt::Key_Option,                 "Option"     },
	{ Qt::Key_Paste,                  "Paste"      },
	{ Qt::Key_Phone,                  "Phone"      },
	{ Qt::Key_Calendar,               "Calendar"   },
	{ Qt::Key_Reply,                  "Reply"      },
	{ Qt::Key_Reload,                 "Reload"     },
	{ Qt::Key_RotateWindows,          "RotateWnds" },
	{ Qt::Key_RotationPB,             "RotationPB" },
	{ Qt::Key_RotationKB,             "RotationKB" },
	{ Qt::Key_Save,                   "Save"       },
	{ Qt::Key_Send,                   "Send"       },
	{ Qt::Key_Spell,                  "Spell"      },
	{ Qt::Key_SplitScreen,            "SpltScreen" },
	{ Qt::Key_Support,                "Support"    },
	{ Qt::Key_TaskPane,               "TaskPane"   },
	{ Qt::Key_Terminal,               "Terminal"   },
	{ Qt::Key_Tools,                  "Tools"      },
	{ Qt::Key_Travel,                 "Travel"     },
	{ Qt::Key_Video,                  "Video"      },
	{ Qt::Key_Word,                   "Word"       },
	{ Qt::Key_Xfer,                   "Xfer"       },
	{ Qt::Key_ZoomIn,                 "ZoomIn"     },
	{ Qt::Key_ZoomOut,                "ZommOut"    },
	{ Qt::Key_Away,                   "Away"       },
	{ Qt::Key_Messenger,              "Messenger"  },
	{ Qt::Key_WebCam,                 "WebCam"     },
	{ Qt::Key_MailForward,            "MailFrwrd"  },
	{ Qt::Key_Pictures,               "Pictures"   },
	{ Qt::Key_Music,                  "Music"      },
	{ Qt::Key_Battery,                "Battery"    },
	{ Qt::Key_Bluetooth,              "Bluetooth"  },
	{ Qt::Key_WLAN,                   "WLAN"       },
	{ Qt::Key_UWB,                    "UWB"        },
	{ Qt::Key_AudioForward,           "AudioFrwrd" },
	{ Qt::Key_AudioRepeat,            "AudioRpeat" },
	{ Qt::Key_AudioRandomPlay,        "AudioRndPl" },
	{ Qt::Key_Subtitle,               "Subtitle"   },
	{ Qt::Key_AudioCycleTrack,        "AudioCycTr" },
	{ Qt::Key_Time,                   "Time"       },
	{ Qt::Key_Hibernate,              "Hibernate"  },
	{ Qt::Key_View,                   "View"       },
	{ Qt::Key_TopMenu,                "TopMenu"    },
	{ Qt::Key_PowerDown,              "PowerDown"  },
	{ Qt::Key_Suspend,                "Suspend"    },
	{ Qt::Key_ContrastAdjust,         "ContrastAd" },
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
	{ Qt::Key_TouchpadToggle,         "TpadToggle" },
	{ Qt::Key_TouchpadOn,             "TpadOn"     },
	{ Qt::Key_TouchpadOff,            "TpadOff"    },
	{ Qt::Key_MicMute,                "MicMute"    },
	{ Qt::Key_Red,                    "Red"        },
	{ Qt::Key_Green,                  "Green"      },
	{ Qt::Key_Yellow,                 "Yellow"     },
	{ Qt::Key_Blue,                   "Blue"       },
	{ Qt::Key_ChannelUp,              "ChannelUp"  },
	{ Qt::Key_ChannelDown,            "ChannelDwn" },
#endif
	{ Qt::Key_MediaLast,              "MediaLast"  },
	{ Qt::Key_unknown,                "unknown"    },
	{ Qt::Key_Call,                   "Call"       },
	{ Qt::Key_Camera,                 "Camera"     },
	{ Qt::Key_CameraFocus,            "CameraFcus" },
	{ Qt::Key_Context1,               "Context1"   },
	{ Qt::Key_Context2,               "Context2"   },
	{ Qt::Key_Context3,               "Context3"   },
	{ Qt::Key_Context4,               "Context4"   },
	{ Qt::Key_Flip,                   "Flip"       },
	{ Qt::Key_Hangup,                 "Hangup"     },
	{ Qt::Key_No,                     "No"         },
	{ Qt::Key_Select,                 "Select"     },
	{ Qt::Key_Yes,                    "Yes"        },
	{ Qt::Key_ToggleCallHangup,       "TglCllHngp" },
	{ Qt::Key_VoiceDial,              "VoiceDial"  },
	{ Qt::Key_LastNumberRedial,       "LastNmbRdl" },
	{ Qt::Key_Execute,                "Execute"    },
	{ Qt::Key_Printer,                "Printer"    },
	{ Qt::Key_Play,                   "Play"       },
	{ Qt::Key_Sleep,                  "Sleep"      },
	{ Qt::Key_Zoom,                   "Zoom"       },
	{ Qt::Key_Cancel,                 "Cancel"     }
};
*/

// -------------------------------- Oggetto base -----------------------------------------

settingsObject::settingsObject(Format f, QString file, int list_ele) :
		QSettings(QString(info.base_folder) + file, f) {
	listEle = list_ele;
	set = &list_settings[listEle];

	for (int i = 0; i < set->count; i++) {
		val << "";
	}
}
settingsObject::~settingsObject() {}
void settingsObject::wr() {
	to_cfg("all");
	wr_all_keys();
}
void settingsObject::wr(QString group) {
	to_cfg(group);
	wr_all_keys();
}
int settingsObject::val_to_int(int index, const char *buffer) {
	bool finded = false;
	int a;

	for (a = 0; a < set->cfg[index].opts.count; a++) {
		if (QString(buffer) == QString(set->cfg[index].opts.opt[a].sname)) {
			finded = true;
			break;
		}
	}

	if (finded == true) {
		return (set->cfg[index].opts.opt[a].value);
	}

	return (-1);
}
void settingsObject::setup() {
	rd();
	after_the_defaults();
	wr();
}
void settingsObject::to_cfg(QString group) {}
void settingsObject::fr_cfg(QString group) {}
void settingsObject::after_the_defaults() {}
void settingsObject::rd() {
	rd("all");
}
void settingsObject::rd(QString group) {
	for (int i = 0; i < set->count; i++) {
		if (group == "all") {
			rd_key(i);
		} else {
			rd_key(group, i);
		}
	}
	fr_cfg(group);
}
int settingsObject::val_to_int(int index) {
	int value;

	if ((value = val_to_int(index, val.at(index).toLocal8Bit().data())) < 0) {
		val.replace(index, set->cfg[index].def);
		value = val_to_int(index, val.at(index).toLocal8Bit().data());
	}

	return (value);
}
void settingsObject::int_to_val(int index, int value) {
	for (int i = 0; i < set->cfg[index].opts.count; i++) {
		if (set->cfg[index].opts.opt[i].value == value) {
			val.replace(index, set->cfg[index].opts.opt[i].sname);
			break;
		}
	}
}
void settingsObject::cpy_val_to_char(int index, char *dst, int length) {
	memset(dst, 0x00, length);
	if (val.at(index).isEmpty() || val.at(index).isNull()) {
		return;
	}
	strncpy(dst, qPrintable(val.at(index)), length - 1);
}
void settingsObject::char_to_val(int index, char *src) {
	val.replace(index, src);
}
void settingsObject::rd_key(int index) {
	beginGroup(set->cfg[index].grp);
	val.replace(index, value(set->cfg[index].key, set->cfg[index].def).toString());
	endGroup();
}
void settingsObject::rd_key(QString group, int index) {
	if (QString(set->cfg[index].grp) == group) {
		rd_key(index);
	}
}
void settingsObject::wr_key(int index) {
	beginGroup(set->cfg[index].grp);
	setValue(set->cfg[index].key, val.at(index));
	endGroup();
}
void settingsObject::wr_key(QString group, int index) {
	if (QString(set->cfg[index].grp) == group) {
		wr_key(index);
	}
}
void settingsObject::wr_all_keys() {
	s.list = listEle;

	for (int i = 0; i < set->count; i++) {
		wr_key(i);
	}

	sync();
}

// ------------------------------- Configurazioni ----------------------------------------

setObject::setObject(Format f, QString file, int list_ele) : settingsObject(f, file, list_ele) {
	setup();
}
setObject::~setObject() {}
double setObject::val_to_double(WORD round, const char *buffer) {
	WORD value = atoi(buffer);
	double ret = 0;

	if (round > 0) {
		value = emu_round_WORD(value, round);\
	}

	ret = ((double) value) / 100.0f;

	if (ret > 1.0f) {
		ret = 1.0f;
	}

	return (ret);
}
double setObject::val_to_double(int index, WORD round) {
	return (val_to_double(round, val.at(index).toLocal8Bit().data()));
}
void setObject::oscan_val_to_int(int index, _overscan_borders *ob, const char *buffer) {
	QStringList splitted = QString(buffer).split(",");

	if (splitted.count() >= 1) {
		ob->up = splitted.at(0).toInt();
	}
	if (splitted.count() >= 2) {
		ob->down = splitted.at(1).toInt();
	}
	if (splitted.count() >= 3) {
		ob->left = splitted.at(2).toInt();
	}
	if (splitted.count() >= 4) {
		ob->right = splitted.at(3).toInt();
	}
}
void setObject::oscan_default(_overscan_borders *ob, BYTE mode) {
	QStringList def;

	if (mode == NTSC) {
		def = QString(set->cfg[SET_OVERSCAN_BRD_NTSC].def).split(",");
	} else {
		def = QString(set->cfg[SET_OVERSCAN_BRD_PAL].def).split(",");
	}

	ob->up = def.at(0).toInt();
	ob->down = def.at(1).toInt();
	ob->left = def.at(2).toInt();
	ob->right = def.at(3).toInt();
}
void setObject::setup() {
	/* attivo la modalita' configurazione */
	info.on_cfg = TRUE;

	settingsObject::setup();

	text_add_line_info(1, "configuration [green]loaded");
}
void setObject::to_cfg(QString group) {
	if ((group == "system") || (group == "all")) {
		int_to_val(SET_MODE, cfg_from_file.mode);
		int_to_val(SET_BCK_PAUSE, cfg_from_file.bck_pause);
		int_to_val(SET_CHEAT_MODE, cfg_from_file.cheat_mode);
		int_to_val(SET_SAVE_SETTINGS_ON_EXIT, cfg_from_file.save_on_exit);
	}

	if ((group == "video") || (group == "all")) {
		int_to_val(SET_RENDERING, cfg_from_file.render);
		int_to_val(SET_FPS, cfg_from_file.fps);
		int_to_val(SET_FRAMESKIP, cfg_from_file.frameskip);
		int_to_val(SET_SCALE, cfg_from_file.scale);
		int_to_val(SET_PAR, cfg_from_file.pixel_aspect_ratio);
		int_to_val(SET_PAR_SOFT_STRETCH, cfg_from_file.PAR_soft_stretch);
		int_to_val(SET_OVERSCAN_DEFAULT, cfg_from_file.oscan_default);
		val.replace(SET_OVERSCAN_BRD_NTSC, oscan_val(&overscan_borders[0]));
		val.replace(SET_OVERSCAN_BRD_PAL, oscan_val(&overscan_borders[1]));
		int_to_val(SET_FILTER, cfg_from_file.filter);
		int_to_val(SET_NTSC_FORMAT, cfg_from_file.ntsc_format);
		int_to_val(SET_PALETTE, cfg_from_file.palette);
		char_to_val(SET_FILE_PALETTE, cfg_from_file.palette_file);
		int_to_val(SET_SWAP_EMPHASIS_PAL, cfg_from_file.disable_swap_emphasis_pal);
		int_to_val(SET_VSYNC, cfg_from_file.vsync);
		int_to_val(SET_INTERPOLATION, cfg_from_file.interpolation);
		int_to_val(SET_TEXT_ON_SCREEN, cfg_from_file.txt_on_screen);
		int_to_val(SET_FULLSCREEN, cfg_from_file.fullscreen);
		int_to_val(SET_STRETCH_FULLSCREEN, cfg_from_file.stretch);
	}

	if ((group == "audio") || (group == "all")) {
		int_to_val(SET_SAMPLERATE, cfg_from_file.samplerate);
		int_to_val(SET_CHANNELS, cfg_from_file.channels);
		double_to_val(SET_STEREO_DELAY, cfg_from_file.stereo_delay);
		int_to_val(SET_AUDIO_QUALITY, cfg_from_file.audio_quality);
		int_to_val(SET_SWAP_DUTY, cfg_from_file.swap_duty);
		int_to_val(SET_AUDIO, cfg_from_file.apu.channel[APU_MASTER]);
	}

	if ((group == "GUI") || (group == "all")) {
		char_to_val(SET_GUI_OPEN_PATH, gui.last_open_path);
		val.replace(SET_GUI_LAST_POSITION, lastpos_val());
		int_to_val(SET_GUI_LANGUAGE, cfg_from_file.language);
	}

	if ((group == "apu channels") || (group == "all")) {
		val.replace(SET_APU_MASTER, channel_val(SET_APU_MASTER));
		val.replace(SET_APU_SQUARE1, channel_val(SET_APU_SQUARE1));
		val.replace(SET_APU_SQUARE2, channel_val(SET_APU_SQUARE2));
		val.replace(SET_APU_TRIANGLE, channel_val(SET_APU_TRIANGLE));
		val.replace(SET_APU_NOISE, channel_val(SET_APU_NOISE));
		val.replace(SET_APU_DMC, channel_val(SET_APU_DMC));
		val.replace(SET_APU_EXTRA, channel_val(SET_APU_EXTRA));
	}
}
void setObject::fr_cfg(QString group) {
	if ((group == "system") || (group == "all")) {
		cfg_from_file.mode = val_to_int(SET_MODE);
		cfg_from_file.bck_pause = val_to_int(SET_BCK_PAUSE);
		cfg_from_file.cheat_mode = val_to_int(SET_CHEAT_MODE);
		cfg_from_file.save_on_exit = val_to_int(SET_SAVE_SETTINGS_ON_EXIT);
	}

	if ((group == "video") || (group == "all")) {
		cfg_from_file.render = val_to_int(SET_RENDERING);
		cfg_from_file.fps = val_to_int(SET_FPS);
		cfg_from_file.frameskip = val_to_int(SET_FRAMESKIP);
		cfg_from_file.scale = val_to_int(SET_SCALE);
		cfg_from_file.pixel_aspect_ratio = val_to_int(SET_PAR);
		cfg_from_file.PAR_soft_stretch = val_to_int(SET_PAR_SOFT_STRETCH);
		cfg_from_file.oscan_default = val_to_int(SET_OVERSCAN_DEFAULT);
		oscan_val_to_int(SET_OVERSCAN_BRD_NTSC, &overscan_borders[0]);
		oscan_val_to_int(SET_OVERSCAN_BRD_PAL, &overscan_borders[1]);
		cfg_from_file.filter = val_to_int(SET_FILTER);
		cfg_from_file.ntsc_format = val_to_int(SET_NTSC_FORMAT);
		cfg_from_file.palette = val_to_int(SET_PALETTE);
		cpy_val_to_char(SET_FILE_PALETTE, cfg_from_file.palette_file,
				sizeof(cfg_from_file.palette_file));
		cfg_from_file.disable_swap_emphasis_pal = val_to_int(SET_SWAP_EMPHASIS_PAL);
		cfg_from_file.vsync = val_to_int(SET_VSYNC);
		cfg_from_file.interpolation = val_to_int(SET_INTERPOLATION);
		cfg_from_file.txt_on_screen = val_to_int(SET_TEXT_ON_SCREEN);
		cfg_from_file.fullscreen = val_to_int(SET_FULLSCREEN);
		cfg_from_file.stretch = val_to_int(SET_STRETCH_FULLSCREEN);
	}

	if ((group == "audio") || (group == "all")) {
		cfg_from_file.samplerate = val_to_int(SET_SAMPLERATE);
		cfg_from_file.channels = val_to_int(SET_CHANNELS);
		cfg_from_file.stereo_delay = val_to_double(SET_STEREO_DELAY, 5);
		cfg_from_file.audio_quality = val_to_int(SET_AUDIO_QUALITY);
		cfg_from_file.swap_duty = val_to_int(SET_SWAP_DUTY);
		cfg_from_file.apu.channel[APU_MASTER] = val_to_int(SET_AUDIO);
	}

	if ((group == "GUI") || (group == "all")) {
		cpy_val_to_char(SET_GUI_OPEN_PATH, gui.last_open_path, sizeof(gui.last_open_path));
		lastpos_val_to_int(SET_GUI_LAST_POSITION);
		cfg_from_file.language = val_to_int(SET_GUI_LANGUAGE);
	}

	if ((group == "apu channels") || (group == "all")) {
		channel_val_to_int(SET_APU_MASTER);
		channel_val_to_int(SET_APU_SQUARE1);
		channel_val_to_int(SET_APU_SQUARE2);
		channel_val_to_int(SET_APU_TRIANGLE);
		channel_val_to_int(SET_APU_NOISE);
		channel_val_to_int(SET_APU_DMC);
		channel_val_to_int(SET_APU_EXTRA);
	}
}
void setObject::after_the_defaults() {
	machine = machinedb[NTSC - 1];
	gfx.scale_before_fscreen = cfg_from_file.scale;
	gfx_set_render(cfg_from_file.render);

	save_slot.slot = 0;
	cfg_from_file.oscan = OSCAN_DEFAULT;

	if (cfg_from_file.cheat_mode == GAMEGENIE_MODE) {
		gamegenie_check_rom_present(TRUE);
	}

	overscan.borders = &overscan_borders[0];
}
void setObject::double_to_val(int index, double value) {
	val.replace(index, QString().setNum((int) (value * 100.0f)));
}
void setObject::lastpos_val_to_int(int index) {
	QStringList splitted = QString(val.at(index)).split(",");

	if (splitted.count() >= 1) {
		cfg->last_pos.x = splitted.at(0).toInt();
	}
	if (splitted.count() >= 2) {
		cfg->last_pos.y = splitted.at(1).toInt();
	}

	val.replace(index, lastpos_val());
}
QString setObject::lastpos_val() {
	return (QString("%1,").arg(cfg->last_pos.x) +
			QString("%1").arg(cfg->last_pos.y));
}
void setObject::oscan_val_to_int(int index, _overscan_borders *ob) {
	if (index == SET_OVERSCAN_BRD_NTSC) {
		oscan_default(ob, NTSC);
	} else {
		oscan_default(ob, PAL);
	}

	oscan_val_to_int(index, ob, val.at(index).toLocal8Bit().data());

	val.replace(index, oscan_val(ob));
}
QString setObject::oscan_val(_overscan_borders *ob) {
	return (QString("%1,").arg(ob->up) +
			QString("%1,").arg(ob->down) +
			QString("%1,").arg(ob->left) +
			QString("%1").arg(ob->right));
}
int setObject::channel_convert_index(int index) {
	switch (index) {
		default:
		case SET_APU_MASTER:
			return (APU_MASTER);
		case SET_APU_SQUARE1:
			return (APU_S1);
		case SET_APU_SQUARE2:
			return (APU_S2);
		case SET_APU_TRIANGLE:
			return (APU_TR);
		case SET_APU_NOISE:
			return (APU_NS);
		case SET_APU_DMC:
			return (APU_DMC);
		case SET_APU_EXTRA:
			return (APU_EXTRA);
	}
}
void setObject::channel_decode(int index, QString val) {
	QStringList split = val.split(",");

	if (split.count() != 2) {
		return;
	}

	cfg_from_file.apu.channel[channel_convert_index(index)] = 0;

	if (split.at(0) == "on") {
		cfg_from_file.apu.channel[channel_convert_index(index)] = 1;
	}

	cfg_from_file.apu.volume[channel_convert_index(index)] = split.at(1).toDouble() / 100.0f;
}
void setObject::channel_default(int index) {
	channel_decode(index, set->cfg[index].def);
}
void setObject::channel_val_to_int(int index) {
	QStringList splitted = val.at(index).split(",");

	channel_default(index);
	channel_decode(index, val.at(index));

	val.replace(index, channel_val(index));
}
QString setObject::channel_val(int index) {
	return (QString(cfg_from_file.apu.channel[channel_convert_index(index)] == 1 ? "on" : "off") +
			QString(",%1").arg(cfg_from_file.apu.volume[channel_convert_index(index)] * 100));
}

// ---------------------------------------- Per game -------------------------------------

pgsObject::pgsObject(Format f, QString file, int list_ele) : settingsObject(f, file, list_ele) {
	setup();
}
pgsObject::~pgsObject() {}
void pgsObject::setup() {
	rd();

	if (QFile(fileName()).exists()) {
		text_add_line_info(1, "pgs [green]loaded");
		wr();
	}
}
void pgsObject::to_cfg(QString group) {
	int_to_val(SET_PGS_SLOT, save_slot.slot);
	char_to_val(SET_PGS_FILE_SAVE, cfg_from_file.save_file);
	int_to_val(SET_PGS_OVERSCAN, cfg_from_file.oscan);
}
void pgsObject::fr_cfg(QString group) {
	save_slot.slot = val_to_int(SET_PGS_SLOT);
	cpy_val_to_char(SET_PGS_FILE_SAVE, cfg_from_file.save_file, sizeof(cfg_from_file.save_file));
	cfg_from_file.oscan = val_to_int(SET_PGS_OVERSCAN);
}

// ----------------------------------------- Input ---------------------------------------

inpObject::inpObject(Format f, QString file, int list_ele) : settingsObject(f, file, list_ele) {
	setup();
}
inpObject::~inpObject() {}
QString inpObject::kbd_keyval_to_name(const DBWORD value) {
	bool ok = false;
	int index = 0;

	if (!value) {
		return ("NULL");
	}

	for (unsigned int i = 0; i < LENGTH(kvSpecials); i++) {
		if (value == kvSpecials[i].value) {
			ok = true;
			index = i;
			break;
		}
	}

	if (ok) {
		return (QString(kvSpecials[index].name));
	}

	return (QKeySequence(value).toString(QKeySequence::PortableText));
}
DBWORD inpObject::kbd_keyval_decode(QKeyEvent *keyEvent) {
	DBWORD key = keyEvent->key();
	quint32 native = keyEvent->nativeVirtualKey();

#if defined (__WIN32__)
#define EXTENDED_KEY_MASK 0x01000000
#define LSHIFT_MASK       0x2A
#define RSHIFT_MASK       0X36
	bool extended = (bool) (keyEvent->nativeModifiers() & EXTENDED_KEY_MASK);

	switch (native) {
		case VK_CONTROL:
			native = extended ? VK_RCONTROL : VK_LCONTROL;
			break;
		case VK_MENU:
			native = extended ? VK_RMENU : VK_LMENU;
			break;
		case VK_SHIFT: {
			quint32 scode = keyEvent->nativeScanCode();

			if ((scode ^ LSHIFT_MASK) == 0) {
				native = VK_LSHIFT;
			} else if ((scode ^ RSHIFT_MASK) == 0) {
				native = VK_RSHIFT;
			}
			break;
		}
		default:
			break;
	};
#undef EXTENDED_KEY_MASK
#undef LSHIFT_MASK
#undef RSHIFT_MASK
#endif

	for (unsigned int i = 0; i < LENGTH(kvSpecials); i++) {
		if (key == kvSpecials[i].key) {
			if (kvSpecials[i].native) {
				if (native != kvSpecials[i].native) {
					continue;
				}
				key = kvSpecials[i].value;
				break;
			} else if (kvSpecials[i].modifiers == Qt::NoModifier) {
				key = kvSpecials[i].value;
				break;
			} else if (keyEvent->modifiers() == kvSpecials[i].modifiers) {
				key = kvSpecials[i].value;
				break;
			}
		}
	}

	return (key);
}
void inpObject::set_kbd_joy_default(_port *port, int index, int mode) {
	int pIndex = 0;

	switch (index) {
		case PORT1:
			if (mode == KEYBOARD) {
				pIndex = SET_INP_P1K_A;
			} else {
				pIndex = SET_INP_P1J_A;
			}
			break;
		case PORT2:
			if (mode == KEYBOARD) {
				pIndex = SET_INP_P2K_A;
			} else {
				pIndex = SET_INP_P2J_A;
			}
			break;
		case PORT3:
			if (mode == KEYBOARD) {
				pIndex = SET_INP_P3K_A;
			} else {
				pIndex = SET_INP_P3J_A;
			}
			break;
		case PORT4:
			if (mode == KEYBOARD) {
				pIndex = SET_INP_P4K_A;
			} else {
				pIndex = SET_INP_P4J_A;
			}
			break;
	}

	for (int i = BUT_A; i < MAX_STD_PAD_BUTTONS; i++) {
		if (mode == KEYBOARD) {
			port->input[KEYBOARD][i] = kbd_keyval_from_name(pIndex + i, set->cfg[pIndex + i].def);
		} else {
			port->input[JOYSTICK][i] = name_to_jsv(set->cfg[pIndex + i].def);
		}
	}
}
void inpObject::set_all_input_default(_config_input *config_input, _array_pointers_port *array) {
	config_input->permit_updown_leftright = FALSE;
	config_input->controller_mode = CTRL_MODE_NES;

	for (int i = PORT1; i < PORT_MAX; i++) {
		_port *port = array->port[i];

		switch (i) {
			case PORT1:
				port->type = CTRL_STANDARD;
				port->joy_id = name_to_jsn("JOYSTICKID1");
				break;
			case PORT2:
				port->type = FALSE;
				port->joy_id = name_to_jsn("JOYSTICKID2");
				break;
			default:
				port->type = FALSE;
				port->joy_id = name_to_jsn("NULL");
				break;
		}

		port->type_pad = CTRL_PAD_AUTO;

		port->turbo[TURBOA].frequency = TURBO_BUTTON_DELAY_DEFAULT;
		port->turbo[TURBOB].frequency = TURBO_BUTTON_DELAY_DEFAULT;
		set_kbd_joy_default(port, i, KEYBOARD);
		set_kbd_joy_default(port, i, JOYSTICK);
	}
}
void *inpObject::sc_val_to_qstring_pntr(int index, int type) {
	static QString str;

	str = val.at(index).split(",").at(type);;
	return ((void *)&str);
}
void inpObject::sc_qstring_pntr_to_val(void *str, int index, int type) {
	QStringList split = val.at(index).split(",");

	split.replace(type, (QString(*(QString *)str)));
	val.replace(index, QString("%1,%2").arg(split.at(KEYBOARD), split.at(JOYSTICK)));
}
void inpObject::setup() {
	rd();
	wr();
}
void inpObject::to_cfg(QString group) {
	if ((group == "port 1") || (group == "all")) {
		int_to_val(SET_INP_P1_CONTROLLER, port[PORT1].type);
		int_to_val(SET_INP_P1_PAD_TYPE, port[PORT1].type_pad);
		kbd_wr(SET_INP_P1K_A, PORT1);
		joy_wr(SET_INP_P1J_A, PORT1);
		joyid_int_to_val(SET_INP_P1J_ID, port[PORT1].joy_id);
		val.replace(SET_INP_P1_TURBOA_DELAY, QString::number(port[PORT1].turbo[TURBOA].frequency));
		val.replace(SET_INP_P1_TURBOB_DELAY, QString::number(port[PORT1].turbo[TURBOB].frequency));
	}

	if ((group == "port 2") || (group == "all")) {
		int_to_val(SET_INP_P2_CONTROLLER, port[PORT2].type);
		int_to_val(SET_INP_P2_PAD_TYPE, port[PORT2].type_pad);
		kbd_wr(SET_INP_P2K_A, PORT2);
		joy_wr(SET_INP_P2J_A, PORT2);
		joyid_int_to_val(SET_INP_P2J_ID, port[PORT2].joy_id);
		val.replace(SET_INP_P2_TURBOA_DELAY, QString::number(port[PORT2].turbo[TURBOA].frequency));
		val.replace(SET_INP_P2_TURBOB_DELAY, QString::number(port[PORT2].turbo[TURBOB].frequency));
	}

	if ((group == "port 3") || (group == "all")) {
		int_to_val(SET_INP_P3_CONTROLLER, port[PORT3].type);
		int_to_val(SET_INP_P3_PAD_TYPE, port[PORT3].type_pad);
		kbd_wr(SET_INP_P3K_A, PORT3);
		joy_wr(SET_INP_P3J_A, PORT3);
		joyid_int_to_val(SET_INP_P3J_ID, port[PORT3].joy_id);
		val.replace(SET_INP_P3_TURBOA_DELAY, QString::number(port[PORT3].turbo[TURBOA].frequency));
		val.replace(SET_INP_P3_TURBOB_DELAY, QString::number(port[PORT3].turbo[TURBOB].frequency));
	}

	if ((group == "port 4") || (group == "all")) {
		int_to_val(SET_INP_P4_CONTROLLER, port[PORT4].type);
		int_to_val(SET_INP_P4_PAD_TYPE, port[PORT4].type_pad);
		kbd_wr(SET_INP_P4K_A, PORT4);
		joy_wr(SET_INP_P4J_A, PORT4);
		joyid_int_to_val(SET_INP_P4J_ID, port[PORT4].joy_id);
		val.replace(SET_INP_P4_TURBOA_DELAY, QString::number(port[PORT4].turbo[TURBOA].frequency));
		val.replace(SET_INP_P4_TURBOB_DELAY, QString::number(port[PORT4].turbo[TURBOB].frequency));
	}

	if ((group == "system") || (group == "all")) {
		int_to_val(SET_INP_CONTROLLER_MODE, cfg_from_file.input.controller_mode);
		int_to_val(SET_INP_LEFTRIGHT, cfg_from_file.input.permit_updown_leftright);
	}

	if ((group == "shortcuts") || (group == "all")) {
		joyid_int_to_val(SET_INP_SC_JOYSTICK_ID, cfg_from_file.input.shcjoy_id);
	}

	if ((group == "special keys") || (group == "all")) {
		val.replace(SET_INP_SK_TIMELINE_KEY, kbd_keyval_to_name(gui.key.tl));
		val.replace(SET_INP_SK_DOUBLE_SPEED_KEY, kbd_keyval_to_name(gui.key.speed));
	}
}
void inpObject::fr_cfg(QString group) {
	if ((group == "port 1") || (group == "all")) {
		port[PORT1].type = val_to_int(SET_INP_P1_CONTROLLER);
		port[PORT1].type_pad = val_to_int(SET_INP_P1_PAD_TYPE);
		kbd_rd(SET_INP_P1K_A, PORT1);
		joy_rd(SET_INP_P1J_A, PORT1);
		port[PORT1].joy_id = joyid_val_to_int(SET_INP_P1J_ID);
		port[PORT1].turbo[TURBOA].frequency = tb_delay_val_to_int(SET_INP_P1_TURBOA_DELAY);
		port[PORT1].turbo[TURBOB].frequency = tb_delay_val_to_int(SET_INP_P1_TURBOB_DELAY);
	}

	if ((group == "port 2") || (group == "all")) {
		port[PORT2].type = val_to_int(SET_INP_P2_CONTROLLER);
		port[PORT2].type_pad = val_to_int(SET_INP_P2_PAD_TYPE);
		kbd_rd(SET_INP_P2K_A, PORT2);
		joy_rd(SET_INP_P2J_A, PORT2);
		port[PORT2].joy_id = joyid_val_to_int(SET_INP_P2J_ID);
		port[PORT2].turbo[TURBOA].frequency = tb_delay_val_to_int(SET_INP_P2_TURBOA_DELAY);
		port[PORT2].turbo[TURBOB].frequency = tb_delay_val_to_int(SET_INP_P2_TURBOB_DELAY);
	}

	if ((group == "port 3") || (group == "all")) {
		port[PORT3].type = val_to_int(SET_INP_P3_CONTROLLER);
		port[PORT3].type_pad = val_to_int(SET_INP_P3_PAD_TYPE);
		kbd_rd(SET_INP_P3K_A, PORT3);
		joy_rd(SET_INP_P3J_A, PORT3);
		port[PORT3].joy_id = joyid_val_to_int(SET_INP_P3J_ID);
		port[PORT3].turbo[TURBOA].frequency = tb_delay_val_to_int(SET_INP_P3_TURBOA_DELAY);
		port[PORT3].turbo[TURBOB].frequency = tb_delay_val_to_int(SET_INP_P3_TURBOB_DELAY);
	}

	if ((group == "port 4") || (group == "all")) {
		port[PORT4].type = val_to_int(SET_INP_P4_CONTROLLER);
		port[PORT4].type_pad = val_to_int(SET_INP_P4_PAD_TYPE);
		kbd_rd(SET_INP_P4K_A, PORT4);
		joy_rd(SET_INP_P4J_A, PORT4);
		port[PORT4].joy_id = joyid_val_to_int(SET_INP_P4J_ID);
		port[PORT4].turbo[TURBOA].frequency = tb_delay_val_to_int(SET_INP_P4_TURBOA_DELAY);
		port[PORT4].turbo[TURBOB].frequency = tb_delay_val_to_int(SET_INP_P4_TURBOB_DELAY);
	}

	if ((group == "system") || (group == "all")) {
		cfg_from_file.input.controller_mode = val_to_int(SET_INP_CONTROLLER_MODE);
		cfg_from_file.input.permit_updown_leftright = val_to_int(SET_INP_LEFTRIGHT);
	}

	if ((group == "shortcuts") || (group == "all")) {
		cfg_from_file.input.shcjoy_id = joyid_val_to_int(SET_INP_SC_JOYSTICK_ID);

		// converto nel nuovo formato
		for (int i = SET_INP_SC_OPEN; i < SET_INP_SC_OPEN + SET_MAX_NUM_SC; i++) {
			QStringList split = val.at(i).split(",");

			if (split.count() < 2) {
				val.replace(i, QString("%1,%2").arg(split.at(KEYBOARD), "NULL"));
			}
		}
	}

	if ((group == "special keys") || (group == "all")) {
		gui.key.tl = kbd_val_to_int(SET_INP_SK_TIMELINE_KEY);
		gui.key.speed = kbd_val_to_int(SET_INP_SK_DOUBLE_SPEED_KEY);
	}
}
int inpObject::kbd_val_to_int(int index) {
	if (val.at(index).isEmpty()) {
		val.replace(index, set->cfg[index].def);
	}

	return (kbd_keyval_from_name(index, val.at(index)));
}
void inpObject::kbd_rd(int index, int pIndex) {
	for (int i = BUT_A; i < MAX_STD_PAD_BUTTONS; i++) {
		port[pIndex].input[KEYBOARD][i] = kbd_val_to_int(index + i);
	}
}
void inpObject::kbd_wr(int index, int pIndex) {
	for (int i = BUT_A; i < MAX_STD_PAD_BUTTONS; i++) {
		val.replace(index + i, kbd_keyval_to_name(port[pIndex].input[KEYBOARD][i]));
	}
}
DBWORD inpObject::kbd_name(QString name) {
	DBWORD value = QKeySequence::fromString(name).operator[](0);

	for (unsigned int i = 0; i < LENGTH(kvSpecials); i++) {
		if (name == kvSpecials[i].name) {
			value = (kvSpecials[i].value);
			break;
		}
	}

	return (value);
}
DBWORD inpObject::kbd_keyval_from_name(int index, QString name) {
	DBWORD value = 0;

	if ((name != "NULL") && !(value = kbd_name(name))) {
		value = kbd_name(set->cfg[index].def);
	}

	return (value);
}
int inpObject::joy_val_to_int(int index) {
	if (val.at(index).isEmpty()) {
		val.replace(index, set->cfg[index].def);
	}

	return (name_to_jsv(val.at(index).toLocal8Bit().data()));
}
void inpObject::joy_rd(int index, int pIndex) {
	for (int i = BUT_A; i < MAX_STD_PAD_BUTTONS; i++) {
		port[pIndex].input[JOYSTICK][i] = joy_val_to_int(index + i);
	}
}
void inpObject::joy_wr(int index, int pIndex) {
	for (int i = BUT_A; i < MAX_STD_PAD_BUTTONS; i++) {
		val.replace(index + i, jsv_to_name(port[pIndex].input[JOYSTICK][i]));
	}
}
int inpObject::joyid_val_to_int(int index) {
	if (val.at(index).isEmpty()) {
		val.replace(index, set->cfg[index].def);
	}

	return (name_to_jsn(val.at(index).toLocal8Bit().data()));
}
void inpObject::joyid_int_to_val(int index, int id) {
	val.replace(index, jsn_to_name(id));
}
int inpObject::tb_delay_val_to_int(int index) {
	int ret;

	if (val.at(index).isEmpty()) {
		ret = TURBO_BUTTON_DELAY_DEFAULT;
	} else {
		ret = val.at(index).toInt();
	}

	if (ret > TURBO_BUTTON_DELAY_MAX) {
		ret = TURBO_BUTTON_DELAY_MAX;
	}

	return (ret);
}

// ----------------------------------------- I/O------------------------------------------

bool rd_cfg_file(QIODevice &device, QSettings::SettingsMap &map) {
	QTextStream in(&device);
	const _list_settings *cfg = &list_settings[s.list];

	in.setCodec("UTF-8");

	while (!in.atEnd()) {
		QString line = in.readLine();

		{
			//QByteArray ba = line.toAscii();
			QByteArray ba = line.toLatin1();
			char *start = ba.data();
			char *end = start + ba.length() - 1;

			if (((*start) == '#') || (ba.length() == 0) ||(((*start) == '[') && ((*end) == ']'))) {
				continue;
			}
		}

		{
			QStringList splitted = line.split("=");
			QString group, key, value;
			bool key_is_good = false;

			for (int b = 0; b < cfg->count; b++) {
				// elimino eventuali spazi finali
				key = QString(splitted.at(0)).replace(QRegExp("\\s*$"), "");
				if (key == QString(cfg->cfg[b].key)) {
					group = QString(cfg->cfg[b].grp);
					key_is_good = true;
					break;
				}
			}

			// elimino eventuali spazi iniziali
			value = splitted.at(1).trimmed();

			if (key_is_good) {
				map[group + '/' + key] = value;
			}
		}
	}

	return (true);
}
bool wr_cfg_file(QIODevice &device, const QSettings::SettingsMap &map) {
	QTextStream out(&device);
	QSettings::SettingsMap::const_iterator iter = map.begin();
	QString group;
	const _list_settings *cfg  = &list_settings[s.list];
	int count_grp = 0;

	out.setCodec("UTF-8");

	for (; iter != map.end(); ++iter) {
		if (iter.key().isEmpty() == false) {
			QStringList splitted = iter.key().split("/");
			QString key = splitted.at(1);

			if (group != splitted.at(0)) {
				group = splitted.at(0);
				if (count_grp > 0) {
					out << NEWLINE;
				}
				out << "[" << group.remove("\"") << "]" << NEWLINE;
				count_grp++;
			}

			for (int i = 0; i < cfg->count; i++) {
				if (key == QString(cfg->cfg[i].key)) {
					if (cfg->cfg[i].cmt) {
						out << cfg->cfg[i].cmt << NEWLINE;
					}
					break;
				}
			}

			out << key << "=" << iter.value().toString() << NEWLINE;
		}
	}

	return (true);
}
