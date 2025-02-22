//
//  sm9_encap.c
//  SM9_FREE
//

#include "sm9_algorithm.h"
#include "sm9_utils.h"
#include <stdio.h>
// #include "print_out.h"
#include "sm3.h"
#include "sm4.h"

#define macklen 32

int SM9_Encap(unsigned char *ran, SM9_PK *pk, unsigned char *pKey, SM9_Package *pPack, unsigned int iKeyLen)
{
    miracl *mr_mip;
    big h, y, r;
    ecn *QB;
    zzn12 w;
    unsigned char *id;
    unsigned char *mw;
    int mwlen;
    unsigned char *kout;

    if (!sm9encap) {
        // printf("the sm9 encrypt lib is not init, please run SM9_SET_ENCRYPT function\n");
        return LIB_NOT_INIT;
    }
    mr_mip = GenMiracl(sm9len);
    mir_ctx *mirCtx = get_ctx();

    y  = mirvar(_MIPP_ 0);
    r  = mirvar(_MIPP_ 0);
    QB = epoint_init(_MIPPO_);
    zzn12_mirvar(_MIPP_ &w);

    id = (unsigned char *)malloc(sizeof(unsigned char) * (pk->keylen + 1));
    memcpy(id, pk->pk, pk->keylen);
    memcpy(id + pk->keylen, hid + 2, 1);

    h = Hfun(_MIPP_(char *) id, pk->keylen + 1, pPack->secLevel, 1);

    ecurve_mult(_MIPP_ h, p1G1, QB);
    ecurve_add(_MIPP_ cpG1, QB);

    bytes_to_big(_MIPP_ pPack->secLevel, (char *)ran, r);

    ecurve_mult(_MIPP_ r, QB, QB);
    epoint_get(_MIPP_ QB, h, y);

    big_to_bytes(_MIPP_ pPack->secLevel, h, (char *)pPack->x, TRUE);
    big_to_bytes(_MIPP_ pPack->secLevel, y, (char *)pPack->y, TRUE);

    zzn12_copy(&cGt, &w);
    zzn12_pow(_MIPP_ &w, r);

    mwlen = pk->keylen + pPack->secLevel * (12 + 2);
    mw    = (unsigned char *)malloc(sizeof(unsigned char) * (mwlen));
    memcpy(mw, pPack->x, pPack->secLevel);
    memcpy(mw + pPack->secLevel, pPack->y, pPack->secLevel);
    zzn12_tochar(_MIPP_ &w, mw + pPack->secLevel * 2, sm9len);
    memcpy(mw + pPack->secLevel * (12 + 2), pk->pk, pk->keylen);

    kout = (unsigned char *)malloc(sizeof(unsigned char) * (iKeyLen + macklen));

    KDF((char *)mw, mwlen, (iKeyLen + macklen) * 8, (char *)kout);
    memcpy(pKey, kout, iKeyLen);

    mirkill(h);
    mirkill(y);
    mirkill(r);
    epoint_free(QB);
    zzn12_kill(_MIPP_ &w);
    free(mw);
    free(id);
    free(kout);
    CloseMiracl(mr_mip);
    CloseMirCtx(mirCtx);

    return 0;
}

int SM9_Decap(SM9_PK *pk, SM9_CSK *sk, SM9_Package *pack, unsigned char *pKey, unsigned int iKeyLen)
{
    miracl *mr_mip;
    big x, y;
    epoint *C;
    ecn2 csk;
    zzn12 w;
    zzn2 xx;
    zzn2 yy;
    unsigned char *mw;
    int mwlen;
    unsigned char *kout;

    if (!sm9encap) {
        //    printf("the sm9 encrypt lib is not init, please run SM9_SET_ENCRYPT function\n");
        return LIB_NOT_INIT;
    }

    mr_mip = GenMiracl(sm9len);
    mir_ctx *mirCtx = get_ctx();

    x = mirvar(_MIPP_ 0);
    y = mirvar(_MIPP_ 0);
    ecn2_mirvar(_MIPP_ &csk);
    zzn12_mirvar(_MIPP_ &w);
    zzn2_mirvar(_MIPP_ &xx);
    zzn2_mirvar(_MIPP_ &yy);
    C = epoint_init(_MIPPO_);

    bytes_to_big(_MIPP_ pack->secLevel, (char *)pack->x, x);
    bytes_to_big(_MIPP_ pack->secLevel, (char *)pack->y, y);

    if (!epoint_set(_MIPP_ x, y, 1, C)) {
        mirkill(x);
        mirkill(y);
        epoint_free(C);
        ecn2_kill(_MIPP_ &csk);
        zzn2_kill(_MIPP_ &xx);
        zzn2_kill(_MIPP_ &yy);
        zzn12_kill(_MIPP_ &w);

        CloseMiracl(mr_mip);
        CloseMirCtx(mirCtx);

        return NOT_ON_G1;
    }

    bytes_to_big(_MIPP_ sk->secLevel, (char *)sk->x2, x);
    bytes_to_big(_MIPP_ sk->secLevel, (char *)sk->x1, y);
    zzn2_from_bigs(_MIPP_ x, y, &xx);

    bytes_to_big(_MIPP_ sk->secLevel, (char *)sk->y2, x);
    bytes_to_big(_MIPP_ sk->secLevel, (char *)sk->y1, y);
    zzn2_from_bigs(_MIPP_ x, y, &yy);

    if (!ecn2_set(_MIPP_ &xx, &yy, &csk)) {
        mirkill(x);
        mirkill(y);
        epoint_free(C);
        ecn2_kill(_MIPP_ &csk);
        zzn2_kill(_MIPP_ &xx);
        zzn2_kill(_MIPP_ &yy);
        zzn12_kill(_MIPP_ &w);

        CloseMiracl(mr_mip);
        CloseMirCtx(mirCtx);

        return NOT_ON_G2;
    }

    if (!ecap(_MIPP_ &csk, C, sm9t, &sm9X, &w)) {
        mirkill(x);
        mirkill(y);
        epoint_free(C);
        ecn2_kill(_MIPP_ &csk);
        zzn2_kill(_MIPP_ &xx);
        zzn2_kill(_MIPP_ &yy);
        zzn12_kill(_MIPP_ &w);

        CloseMiracl(mr_mip);
        CloseMirCtx(mirCtx);

        return PAIRING_ERROR;
    }

    mwlen = pk->keylen + sk->secLevel * (12 + 2);
    mw    = (unsigned char *)malloc(sizeof(unsigned char) * (mwlen));
    memcpy(mw, pack->x, pack->secLevel);
    memcpy(mw + pack->secLevel, pack->y, pack->secLevel);
    zzn12_tochar(_MIPP_ &w, mw + pack->secLevel * 2, sm9len);
    memcpy(mw + pack->secLevel * (12 + 2), pk->pk, pk->keylen);

    kout = (unsigned char *)malloc(sizeof(unsigned char) * (iKeyLen + macklen));
    KDF((char *)mw, mwlen, (iKeyLen + macklen) * 8, (char *)kout);

    memcpy(pKey, kout, iKeyLen);

    mirkill(x);
    mirkill(y);
    epoint_free(C);
    ecn2_kill(_MIPP_ &csk);
    zzn2_kill(_MIPP_ &xx);
    zzn2_kill(_MIPP_ &yy);
    zzn12_kill(_MIPP_ &w);

    free(mw);
    free(kout);
    CloseMiracl(mr_mip);
    CloseMirCtx(mirCtx);

    return 0;
}
