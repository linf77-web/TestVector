#include "TestVector.h"

Timing::Timing()
{
    memset(m_tim_name, 0, STR_NAME_MIN);
    m_period = 50;
    m_vsignals.reserve(100);
}

const Timing &Timing::operator=(Timing *other)
{
    if(this == other)
        return *this;
    setName(other->timname());
    setPeriod(other->period());
    setVSignal(other->vsignals());
    return *this;
}

Timing::Timing(Timing* other)
{
    setName(other->timname());
    setPeriod(other->period());
    setVSignal(other->vsignals());
}
Timing::Timing(const char *name, const unsigned int &period, SignalEx* signal) : m_period(period)
{
    m_vsignals.reserve(1024);
    memset(m_tim_name, 0, STR_NAME_MIN);
    memcpy(m_tim_name, name, STR_NAME_MIN);
    m_vsignals.push_back(signal);
}

void Timing::setName(const char* name)
{
    memset(m_tim_name, 0, STR_NAME_MIN);
    memcpy(m_tim_name, name, strlen(name));
}
void Timing::addSignal(SignalEx* signal)
{
     m_vsignals.push_back(signal);
}
void Timing::insertSignal(SignalEx* signal, int pos)
{
    std::vector<SignalEx*>::iterator iter = m_vsignals.begin();
    m_vsignals.insert(iter + pos, signal);
}

int Timing::hasSignalByName(const char* signalName)
{
    if(m_vsignals.empty())
        return -1;
    int sig_size = (int)m_vsignals.size();
    for(int index = 0; index < sig_size; ++index)
        if(0 == strcmp(signal(index)->signame(), signalName))
            return  index;
    return -1;
}

SignalEx* Timing::getSignalByName(const char* signalName)
{
    if (m_vsignals.empty())
        return nullptr;
    int sig_size = (int)m_vsignals.size();
    for (int index = 0; index < sig_size; ++index)
        if (0 == strcmp(signal(index)->signame(), signalName))
            return  signal(index);
    return nullptr;
}
void Timing::deleteSignalByName(const char *signalName)
{
    if(m_vsignals.empty())
        return;
    int sig_size = (int)m_vsignals.size();
    for(int index = 0; index < sig_size; ++index)
        if(0 == strcmp(signal(index)->signame(), signalName)) {
            m_vsignals.erase(m_vsignals.begin() + index);
            return;
        }
}

SignalEx::SignalEx()
{
    memset(m_signal_name, 0, STR_NAME_MIN);
    m_vwfc.reserve(100);
}

SignalEx::SignalEx(const char* name, WFC* wfc)
{
    m_vwfc.reserve(100);
    memset(m_signal_name, 0, STR_NAME_MIN);
    memcpy(m_signal_name, name, strlen(name));
    m_vwfc.push_back(wfc);
}

SignalEx::SignalEx(const char* name, const std::vector<WFC*>& vwfc) :  m_vwfc(vwfc)
{
    memset(m_signal_name, 0, STR_NAME_MIN);
    memcpy(m_signal_name, name, STR_NAME_MIN);
}

SignalEx::SignalEx(SignalEx* other)
{
	setName(other->signame());
    setVWFC(other->vwfc());
}

void SignalEx::setName(const char* name)
{ 
    memset(m_signal_name, 0, STR_NAME_MIN);
    memcpy(m_signal_name, name, strlen(name)); 
}

void SignalEx::addWFC(WFC* wfc)
{
    m_vwfc.push_back(wfc);
}
void SignalEx::insertWFC(WFC* wfc, int pos)
{
    std::vector<WFC*>::iterator iter = m_vwfc.begin();
    m_vwfc.insert(iter + pos, wfc);
}

bool SignalEx::hasWfcByName(const char* wfcName)
{
    if(m_vwfc.empty())
        return false;
    int wfc_size = (int)m_vwfc.size();
    for(int index = 0; index < wfc_size; ++index)
        if(0 == strcmp(wfc(index)->wfcname(), wfcName))
            return true;
    return false;
}

WFC *SignalEx::getWfcByName(const char *wfcName)
{
    if(m_vwfc.empty())
        return nullptr;
    int wfc_size = (int)m_vwfc.size();
    for(int index = 0; index < wfc_size; ++index)
        if(0 == strcmp(wfc(index)->wfcname(), wfcName))
            return wfc(index);
    return nullptr;
}

void SignalEx::deleteWfcByName(const char *wfcName)
{
    if(m_vwfc.empty())
        return;
    int wfc_size = (int)m_vwfc.size();
    for(int index = 0; index < wfc_size; ++index)
        if(0 == strcmp(wfc(index)->wfcname(), wfcName)) {
            m_vwfc.erase(m_vwfc.begin() + index);
            return;
        }
}

const SignalEx& SignalEx::operator= (SignalEx* other)
{
	if (this == other)
		return *this;
    memcpy(m_signal_name, other->signame(), strlen(other->signame()));
    this->setVWFC(other->vwfc());
    return *this;
}

Event::Event()
{
    strcat(m_evt_evt, "0ns");
    strcat(m_evt_expr, "Z");
}

Event::Event(Event* other)
{
    memset(m_evt_evt, 0, STR_NAME_MIN);
    memcpy(m_evt_evt, other->evt(), strlen(other->evt()));
    memset(m_evt_expr, 0, STR_NAME_MIN);
    memcpy(m_evt_expr, other->expr(), strlen(other->expr()));
}
Event::Event(const char* evt, const char* expr)
{
    memset(m_evt_evt, 0, STR_NAME_MIN);
    memcpy(m_evt_evt, evt, strlen(evt));
    memset(m_evt_expr, 0, STR_NAME_MIN);
    memcpy(m_evt_expr, expr, strlen(expr));
}

void Event::setEvt(const char* evt) 
{ 
    memset(m_evt_evt, 0, STR_NAME_MIN);
    memcpy(m_evt_evt, evt, strlen(evt));
}
void Event::setExpr(const char* expr) 
{ 
    memset(m_evt_expr, 0, STR_NAME_MIN);
    memcpy(m_evt_expr, expr, strlen(expr));
}

WFC::WFC()
{
    memset(m_wfc_name, 0, STR_NAME_MIN);
    //m_vevt.reserve(100);
}

WFC::WFC(const char* wfc_name, Event *evt)
{
    memset(m_wfc_name, 0, STR_NAME_MIN);
    memcpy(m_wfc_name, wfc_name, strlen(wfc_name));
    m_vevt.reserve(100);
    m_vevt.push_back(evt);
}

WFC::WFC(const char* wfc_name, const std::vector<Event*>& vevt) : m_vevt(vevt)
{
    memset(m_wfc_name, 0, STR_NAME_MIN);
    memcpy(m_wfc_name, wfc_name, strlen(wfc_name));
}
WFC::WFC(WFC* other)
{
    setWfcName(other->wfcname());
    setvEvent(other->vevt());
}

void WFC::operator= (WFC* other)
{
    setWfcName(other->wfcname());
    setvEvent(other->vevt());
}

void WFC::operator= (const WFC& other)
{
    setWfcName(other.wfcname());
    setvEvent(other.vevt());
}
void WFC::setWfcName(const char* name)
{ 
    memset(m_wfc_name, 0, STR_NAME_MIN);
    memcpy(m_wfc_name, name, strlen(name));
}
void WFC::addEvent(Event *evt)
{
    m_vevt.push_back(evt);
}

void WFC::setvEvent(const std::vector<Event*>& vevt)
{
    m_vevt.assign(vevt.begin(), vevt.end());
}

void WFC::setEventOffset(const int& offset)
{
    char unit[2] = {""};
    char chExpr[STR_NAME_MIN] = { 0 };
    for (auto evt : m_vevt) {
        memset(unit, 0, 2);
        std::string str = std::string(evt->expr());
        int expr = 0;
        unsigned long long pos = str.find("ns");
        if (str.npos != pos) {
            std::string substr = str.substr(0, pos);
            sscanf(substr.c_str(), "%d", &expr);
            strcat(unit, "ns");
        }
        else {
            pos = str.find("us");
            if (str.npos != pos) {
                std::string substr = str.substr(0, pos);
                sscanf(substr.c_str(), "%d", &expr);
                strcat(unit, "us");
            }
        } 
        sscanf(str.c_str(), "%d", &expr);
        expr += offset;
        sprintf(chExpr, "%d", expr);
        strcat(chExpr, unit);
        evt->setExpr(chExpr);
    }
}
PatternItem::PatternItem()
{
    m_period = 0;
    m_vwfcs.reserve(100);
    m_vsigNames.reserve(100);
    memset(m_cmd, 0, STR_NAME_MIN);
    strcat(m_cmd, "Nop");
    memset(m_label, 0, STR_NAME_MIN);
    strcat(m_label, " NULL");
}

PatternItem::PatternItem(PatternItem *other)
{
    this->m_period = other->period();
    setVWfcs(other->vwfcs());
    setVSigNames(other->vsigNames());
    memset(m_cmd, 0, STR_NAME_MIN);
    memset(m_label, 0, STR_NAME_MIN);
    memcpy(m_cmd, other->cmd(), strlen(other->cmd()));
    memcpy(m_label, other->label(), strlen(other->label()));
}
PatternItem::~PatternItem()
{
    m_period = 0;
    m_vwfcs.clear();
    memset(m_cmd, 0, STR_NAME_MIN);
    memset(m_label, 0, STR_NAME_MIN);
}
PatternItem::PatternItem(const int& period, const std::vector<WFC*>& vwfc, const std::vector<std::string>& vsigNames,const char* cmd, const char* label)
{
    m_period = period;
    setVWfcs(vwfc);
    setVSigNames(vsigNames);
    memset(m_cmd, 0, STR_NAME_MIN);
    memset(m_label, 0, STR_NAME_MIN);
    memcpy(m_cmd, cmd, strlen(cmd));
    memcpy(m_label, label, strlen(label));
}

void PatternItem::addWfc(WFC *wfc)
{
    m_vwfcs.push_back(wfc);
}

void PatternItem::addSigName(const char * name)
{
    m_vsigNames.push_back(std::string(name));
}

void PatternItem::setCmd(const char* cmd)
{
    memset(m_cmd, 0, STR_NAME_MIN);
    memcpy(m_cmd, cmd, strlen(cmd));
}
void PatternItem::setLabel(const char* label)
{
    memset(m_label, 0, STR_NAME_MIN);
    memcpy(m_label, label, strlen(label));
}
