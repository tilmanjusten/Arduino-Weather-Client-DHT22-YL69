#include "Arduino.h"
#include "Log.h"
#include <inttypes.h>
#include <stdio.h> // for size_t

Log::Log(uint8_t m)
{
    mode = m;
}

void Log::l(const Printable &message)
{
    if (mode == LOG_MODE_VERBOSE)
    {
        Serial.print(message);
    }
}

void Log::l(const String &message)
{
    if (mode == LOG_MODE_VERBOSE)
    {
        Serial.print(message);
    }
}

void Log::l(const char message[])
{
    if (mode == LOG_MODE_VERBOSE)
    {
        Serial.print(message);
    }
}

void Log::l(char message)
{
    if (mode == LOG_MODE_VERBOSE)
    {
        Serial.print(message);
    }
}

void Log::l(unsigned char message)
{
    if (mode == LOG_MODE_VERBOSE)
    {
        Serial.print(message);
    }
}

void Log::l(int message)
{
    if (mode == LOG_MODE_VERBOSE)
    {
        Serial.print(message);
    }
}

void Log::l(unsigned int message)
{
    if (mode == LOG_MODE_VERBOSE)
    {
        Serial.print(message);
    }
}

void Log::l(long message)
{
    if (mode == LOG_MODE_VERBOSE)
    {
        Serial.print(message);
    }
}

void Log::l(unsigned long message)
{
    if (mode == LOG_MODE_VERBOSE)
    {
        Serial.print(message);
    }
}

void Log::l(float message)
{
    if (mode == LOG_MODE_VERBOSE)
    {
        Serial.print(message);
    }
}

void Log::l(double message)
{
    if (mode == LOG_MODE_VERBOSE)
    {
        Serial.print(message);
    }
}

// Log line

void Log::ln(void)
{
    if (mode == LOG_MODE_VERBOSE)
    {
        Serial.println();
    }
}

void Log::ln(const Printable &message)
{
    if (mode == LOG_MODE_VERBOSE)
    {
        Serial.println(message);
    }
}

void Log::ln(const String &message)
{
    if (mode == LOG_MODE_VERBOSE)
    {
        Serial.println(message);
    }
}

void Log::ln(const char message[])
{
    if (mode == LOG_MODE_VERBOSE)
    {
        Serial.println(message);
    }
}

void Log::ln(char message)
{
    if (mode == LOG_MODE_VERBOSE)
    {
        Serial.println(message);
    }
}

void Log::ln(unsigned char message)
{
    if (mode == LOG_MODE_VERBOSE)
    {
        Serial.println(message);
    }
}

void Log::ln(int message)
{
    if (mode == LOG_MODE_VERBOSE)
    {
        Serial.println(message);
    }
}

void Log::ln(unsigned int message)
{
    if (mode == LOG_MODE_VERBOSE)
    {
        Serial.println(message);
    }
}

void Log::ln(long message)
{
    if (mode == LOG_MODE_VERBOSE)
    {
        Serial.println(message);
    }
}

void Log::ln(unsigned long message)
{
    if (mode == LOG_MODE_VERBOSE)
    {
        Serial.println(message);
    }
}

void Log::ln(float message)
{
    if (mode == LOG_MODE_VERBOSE)
    {
        Serial.println(message);
    }
}

void Log::ln(double message)
{
    if (mode == LOG_MODE_VERBOSE)
    {
        Serial.println(message);
    }
}

void Log::ln(String message)
{
    if (mode == LOG_MODE_VERBOSE)
    {
        Serial.println(message);
    }
}
