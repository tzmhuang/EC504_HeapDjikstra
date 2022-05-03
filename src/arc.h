//
// Created by Pingcheng Dong on 5/3/22.
//

#ifndef FIBHEAP_H_ARC_H
#define FIBHEAP_H_ARC_H

/* Define arc */
struct arc{
    struct arc *next;
    int length;
    int end;
};

#endif //FIBHEAP_H_ARC_H
