/*
 * Relay.h
 *
 *  Created on: Nov 13, 2019
 *      Author: roessing
 */

#ifndef RELAY_H_
#define RELAY_H_

#include "pAM-definitions.h"
#include "wait.h"

void relay_setup();
void Reed_setup();
void set_mode(unsigned char mode);
void activate_relay_irq(void);


/******SET_FUNCTIONS*******/
inline void SET_PLUS9V_RELAY(int i){
    if(i==1){RELAY_PORT_out set_high SET_PLUS9V_PIN;}
    else {RELAY_PORT_out set_low SET_PLUS9V_PIN;}
}
inline void SET_MINUS9V_RELAY(int i){
    if(i==1){RELAY_PORT_out set_high SET_MINUS9V_PIN;}
    else {RELAY_PORT_out set_low SET_MINUS9V_PIN;}
}

inline void RESET_PLUS9V_RELAY(int i){
    if(i==1){RELAY_PORT_out set_high RESET_PLUS9V_PIN;}
    else {RELAY_PORT_out set_low RESET_PLUS9V_PIN;}
}
inline void RESET_MINUS9V_RELAY(int i){
    if(i==1){RELAY_PORT_out set_high RESET_MINUS9V_PIN;}
    else {RELAY_PORT_out set_low RESET_MINUS9V_PIN;}
}
/*
inline void SET_VARISTOR(int i){
    if(i==1){MODE_RELAY_PORT_out set_high SET_VARISTOR_PIN;}
    else{MODE_RELAY_PORT_out set_low SET_VARISTOR_PIN;}
}

inline void RESET_VARISTOR(int i){
    if(i==1){MODE_RELAY_PORT_out set_high RESET_VARISTOR_PIN;}
    else{MODE_RELAY_PORT_out set_low RESET_VARISTOR_PIN;}
}

inline void SET_10K(int i){
    if(i==1){MODE_RELAY_PORT_out set_high SET_10K_PIN;}
    else{MODE_RELAY_PORT_out set_low SET_10K_PIN;}
}
inline void RESET_10K(int i){
    if(i==1){MODE_RELAY_PORT_out set_high RESET_10K_PIN;}
    else{MODE_RELAY_PORT_out set_low RESET_10K_PIN;}
}
inline void SET_100R(int i){
    if(i==1){MODE_RELAY_PORT_out set_high SET_100R_PIN;}
    else{MODE_RELAY_PORT_out set_low SET_100R_PIN;}
}

inline void RESET_100R(int i){
    if(i==1){MODE_RELAY_PORT_out set_high RESET_100R_PIN;}
    else{MODE_RELAY_PORT_out set_low RESET_100R_PIN;}
}

inline void SET_1M(int i){
    if(i==1){MODE_RELAY_PORT_out set_high SET_1M_PIN;}
    else{MODE_RELAY_PORT_out set_low SET_1M_PIN;}
}
inline void RESET_1M(int i){
    if(i==1){MODE_RELAY_PORT_out set_high RESET_1M_PIN;}
    else{MODE_RELAY_PORT_out set_low RESET_1M_PIN;}
}

*/
inline void SET_50M(int i){
    if(i==1){MODE_RELAY_PORT_out set_high SET_50M_PIN;}
    else{MODE_RELAY_PORT_out set_low SET_50M_PIN;}
}
inline void RESET_50M(int i){
    if(i==1){MODE_RELAY_PORT_out set_high RESET_50M_PIN;}
    else{MODE_RELAY_PORT_out set_low RESET_50M_PIN;}
}

inline void SET_500M(int i){
    if(i==1){MODE_RELAY_PORT_out set_high SET_500M_PIN;}
    else{MODE_RELAY_PORT_out set_low SET_500M_PIN;}
}

inline void RESET_500M(int i){
    if(i==1){MODE_RELAY_PORT_out set_high RESET_500M_PIN;}
    else{MODE_RELAY_PORT_out set_low RESET_500M_PIN;}
}


inline void SET_5M(int i){
    if(i==1){MODE_RELAY_PORT_out set_high SET_5M_PIN;}
    else{MODE_RELAY_PORT_out set_low SET_5M_PIN;}
}

inline void RESET_5M(int i){
    if(i==1){MODE_RELAY_PORT_out set_high RESET_5M_PIN;}
    else{MODE_RELAY_PORT_out set_low RESET_5M_PIN;}
}



#endif /* RELAY_H_ */
