#ifndef TESTVECTOR_H
#define TESTVECTOR_H

#ifndef EVTS_MAX
#define EVTS_MAX (4)
#endif // !EVTS_MAX

#ifndef STR_NAME_MIN
#define STR_NAME_MIN (32)
#endif

#ifndef STR_NAME_MAX
#define STR_NAME_MAX (64)
#endif

#ifndef SIGNAL_WFC_CNTS_MAX
#define SIGNAL_WFC_CNTS_MAX (1024)
#endif // !SIGNAL_WCF_CNTS_MAX

#ifndef EVT_CNTS_MAX
#define EVT_CNTS_MAX (16384)
#endif // !EVT_CNTS_MAX

#ifndef CHANNEL_CNTS_MAX
#define CHANNEL_CNTS_MAX (32)
#endif

#define _CRT_SECURE_NO_WARNINGS
#include <QString>
#include <vector>
#include <QMetaType>
#include <QVariant>

class Event
{
public:
    Event();
    Event(Event* other);
    Event(const char* evt, const char* expr);
    ~Event() {}
    const char* evt() const { return m_evt_evt; }
    const char* expr() const { return m_evt_expr; }
    void setEvt(const char* evt);
    void setExpr(const char* expr);
private:
    friend class                            SignalEx;
    char m_evt_evt[STR_NAME_MIN];
    char m_evt_expr[STR_NAME_MIN];
};
class WFC
{
public:
    WFC();
    ~WFC(){}
    WFC(const char* wfc_name, Event* evt);
    WFC(const char* wfc_name, const std::vector<Event*>& vevt);
    WFC(WFC* other);
    void operator= (WFC* other);
    void operator= (const WFC& other);
    void setWfcName(const char* wfc);
    void addEvent(Event* evt);
    void setvEvent(const std::vector<Event*>& vevt);
    void setEventOffset(const int& offset);
    const char* wfcname() const { return m_wfc_name; }
    const Event* evt(int index) { return m_vevt[index]; }
    const std::vector<Event*> vevt() const { return m_vevt; }
private:
    char                                    m_wfc_name[STR_NAME_MIN];
    std::vector<Event*>                     m_vevt;
public:
    friend class                            SignalEx;
};
Q_DECLARE_METATYPE(std::vector<WFC*>);

class SignalEx
{
public:
    SignalEx();
    ~SignalEx(){}
    SignalEx(const char* name, WFC* wfc);
    SignalEx(const char* name, const std::vector<WFC*>& vwfc);
    SignalEx(SignalEx* other);
    void setName(const char* name);
    void addWFC(WFC* wfc);
    void insertWFC(WFC* wfc, int pos);
    void setVWFC(const std::vector<WFC*>& vwfc) { m_vwfc.assign(vwfc.begin(), vwfc.end()); }
    bool hasWfcByName(const char* wfcName);
    WFC* getWfcByName(const char* wfcName);
    void deleteWfcByName(const char* wfcName);
    const SignalEx& operator= (SignalEx* other);
    const char* signame() const { return m_signal_name; }
    WFC* wfc(int index) const { return m_vwfc[index]; }
    std::vector<WFC*> vwfc() const { return m_vwfc; }
private:
    char                                    m_signal_name[STR_NAME_MIN];
    std::vector<WFC*>                       m_vwfc;
public:
    friend class                            Timing;
};
Q_DECLARE_METATYPE(std::vector<SignalEx*>);

class Timing
{
public:
    Timing();
    ~Timing(){}
    Timing(Timing* other);
    Timing(const char* name, const unsigned int& period, SignalEx* signal);
    void setName(const char* name);
    void setPeriod(const unsigned int& period) { m_period = period; }
    void addSignal(SignalEx* signal);
    void setVSignal(const std::vector<SignalEx*>& vsignal) {m_vsignals.assign(vsignal.begin(), vsignal.end());}
    void insertSignal(SignalEx* signal, int pos);
    int hasSignalByName(const char* signalName);
    void deleteSignalByName(const char* signalName);
    const Timing& operator= (Timing* other);
    const char* timname(){ return m_tim_name; }
    int period() const { return m_period; }
    SignalEx* signal(int index) const { return m_vsignals[index]; }
    SignalEx* getSignalByName(const char* sigName);
    std::vector<SignalEx*> vsignals() const { return m_vsignals; }
private:
    char                                    m_tim_name[STR_NAME_MIN];
    int                                     m_period;
    std::vector<SignalEx*>                  m_vsignals;
};
Q_DECLARE_METATYPE(std::vector<Timing*>);

class PatternItem
{
public:
    PatternItem();
    PatternItem(PatternItem* other);
    ~PatternItem();
    PatternItem(const int& period, const std::vector<WFC*>& vevts, const std::vector<std::string>& vsigNames,const char* cmd, const char* label);
    void addWfc(WFC* wfc);
    void addSigName(const char*);
    void setCmd(const char* cmd);
    void setLabel(const char* label);
    void setVWfcs(const std::vector<WFC*>& vwfcs) { m_vwfcs.assign(vwfcs.begin(), vwfcs.end()); }
    void setVSigNames(const std::vector<std::string> vnames) { m_vsigNames.assign(vnames.begin(), vnames.end()); }
    const char* cmd() const { return m_cmd; }
    const char* label() const { return m_label; }
    WFC* wfc(int index) const { return m_vwfcs[index]; }
    std::vector<std::string> vsigNames() const {return m_vsigNames;}
    std::string sigName(int index) const { return m_vsigNames[index]; }
    std::vector<WFC*> vwfcs() const { return m_vwfcs; }
    int period() const { return m_period; }
private:
    std::vector<WFC*>                       m_vwfcs;
    std::vector<std::string>                m_vsigNames;
    int                                     m_period;
    char                                    m_cmd[STR_NAME_MIN];
    char                                    m_label[STR_NAME_MIN];
};
Q_DECLARE_METATYPE(std::vector<PatternItem*>);
#endif // TESTVECTOR_H
