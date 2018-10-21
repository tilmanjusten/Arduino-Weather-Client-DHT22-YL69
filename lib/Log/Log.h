#ifndef Log_h
#define Log_h

#include <Arduino.h>
#include <inttypes.h>
#include <stdio.h> // for size_t

#ifndef LOG_MODE_SILENT
#define LOG_MODE_SILENT 0x0
#endif
#ifndef LOG_MODE_VERBOSE
#define LOG_MODE_VERBOSE 0x1
#endif

#ifndef LOG_MODE
#define LOG_MODE LOG_MODE_VERBOSE
#endif

class Log
{
private:
  int mode;

public:
  Log(uint8_t m);
  void l(const Printable &);
  void l(const String &);
  void l(const char[]);
  void l(char);
  void l(unsigned char);
  void l(int);
  void l(unsigned int);
  void l(long);
  void l(unsigned long);
  void l(float);
  void l(double);
  void l(String);

  void ln(void);
  void ln(const Printable &);
  void ln(const String &);
  void ln(const char[]);
  void ln(char);
  void ln(unsigned char);
  void ln(int);
  void ln(unsigned int);
  void ln(long);
  void ln(unsigned long);
  void ln(float);
  void ln(double);
  void ln(String);
};

#endif