#include <bits/stdc++.h>
using namespace std;
enum MonitorType
{
    Win32,
    Win64,
    Ganglia
};
MonitorType type = Ganglia;
class Monitor
{
  public:
    virtual void getLoad() = 0;
    virtual void getTotalMemory() = 0;
    virtual void getUsedMenory() = 0;
    virtual void getNetworkLatency() = 0;
    Monitor(Display *display);
    virtual ~Monitor();
    void show();

  protected:
    float load, latency;
    long totalMemory, usedMenmory;
    Display *m_display;
};
void Monitor::show()
{
    m_display->show(load, totalMemory, usedMenmory, latency);
}

float Monitor::getLoad()
{
    switch (type)
    {
    case Win32:
        return load;
    case Win64:
        return load;
    case Ganglia:
        return load;
    }
}
int main()
{
    WindowsDisplay display;
    Monitor monitor(&display);
    while (running())
    {
        monitor.getLoad();
        monitor.getTotalMemory();
        monitor.getUsedMenory();
        monitor.getNetworkLatency();
        monitor.show();
        sleep(1000);
    }
    return 0;
}