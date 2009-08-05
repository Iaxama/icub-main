
#include "utils.h"


/************************************************************************/
void xdPort::onRead(Bottle &b)
{
    size_t bLen=b.size();
    size_t n=bLen>7 ? 7 : bLen;

    for (unsigned int i=0; i<n; i++)
        xd[i]=b.get(i).asDouble();
}


/************************************************************************/
void exchangeData::setDesired(const Vector &_xd, const Vector &_qd)
{
    mutex1.wait();
    xd=_xd;
    qd=_qd;
    mutex1.post();
}


/************************************************************************/
void exchangeData::getDesired(Vector &_xd, Vector &_qd)
{
    mutex1.wait();
    _xd=xd;
    _qd=qd;
    mutex1.post();
}


/************************************************************************/
void exchangeData::set_q(const Vector &_q)
{
    mutex2.wait();
    q=_q;
    mutex2.post();
}


/************************************************************************/
Vector exchangeData::get_q()
{
    mutex2.wait();
    Vector _q=q;
    mutex2.post();

    return _q;
}


/************************************************************************/
iCubArm *createArm(const string &partName, bool ctrlTorso)
{
    iCubArm *arm;

    if (partName=="right_arm")
        arm=new iCubArm("right");
    else
        arm=new iCubArm("left");
    
    if (ctrlTorso)
    {
        // release torso blocked links
        arm->releaseLink(0);
        arm->releaseLink(1);
        arm->releaseLink(2);
    }

    return arm;
}


/************************************************************************/
void alignJointsBounds(iKinChain *chain, IControlLimits *limTorso,
                       IControlLimits *limArm)
{
    double min, max;

    for (unsigned int i=0; i<3; i++)
    {   
        limTorso->getLimits(i,&min,&max);

        (*chain)[2-i].setMin((M_PI/180.0)*min);
        (*chain)[2-i].setMax((M_PI/180.0)*max);
    }

    for (unsigned int i=0; i<7; i++)
    {   
        limArm->getLimits(i,&min,&max);

        (*chain)[3+i].setMin((M_PI/180.0)*min);
        (*chain)[3+i].setMax((M_PI/180.0)*max);
    }
}


/************************************************************************/
bool getFeedback(Vector &fb, iKinChain *chain, IEncoders *encTorso,
                 IEncoders *encArm, int nJointsTorso, int nJointsArm,
                 bool ctrlTorso)
{
    static unsigned int tmoCntTorso=0;
    static unsigned int tmoCntArm  =0;

    unsigned int offs=ctrlTorso ? nJointsTorso : 0;    
    
    // filled in reversed order
    Vector fbTorso(nJointsTorso);
    if (encTorso->getEncoders(fbTorso.data()))
    {
        if (ctrlTorso)
            for (int i=0; i<nJointsTorso; i++)
                fb[i]=(M_PI/180.0)*fbTorso[nJointsTorso-i-1];
            else
                for (int i=0; i<nJointsTorso; i++)
                    chain->setBlockingValue(i,(M_PI/180.0)*fbTorso[nJointsTorso-i-1]);

        tmoCntTorso=0;
    }

    Vector fbArm(nJointsArm);
    if (encArm->getEncoders(fbArm.data()))
    {
        for (int i=0; i<nJointsArm; i++)
            fb[offs+i]=(M_PI/180.0)*fbArm[i];

        tmoCntArm=0;
    }

    if (++tmoCntTorso>10 || ++tmoCntArm>10)
        return false;
    else
        return true;
}



