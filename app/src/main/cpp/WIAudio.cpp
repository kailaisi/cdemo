//
// Created by wu on 2020/8/3.
//

#include "WIAudio.h"


WIAudio::~WIAudio() {

}

//线程回调方法
void *decodePlay(void *data){
    WIAudio *audio=(WIAudio*)(data);
    audio->resampleAudio();
    pthread_exit(&audio->playThread);
}


void WIAudio::play() {
    //创建线程
    pthread_creat(&playThread,NULL,decodePlay,this);
}

void WIAudio::resampleAudio() {
    while(plays)
}
