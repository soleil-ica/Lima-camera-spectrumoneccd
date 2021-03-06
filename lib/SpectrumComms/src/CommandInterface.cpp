#include "CommandInterface.h"

using namespace SpectrumComms;


CommandInterface::CommandInterface(GpibComms::GpibConfig gpib_config, Listener* listener) :
    m_comms(gpib_config),
    m_listener(listener)
{
    YAT_TRACE("CommandInterface::CommandInterface");
}

CommandInterface::~CommandInterface()
{
    YAT_TRACE("CommandInterface::~CommandInterface");
}

std::string CommandInterface::command_and_wait(const Command & cmd, size_t timeout,
    std::vector<std::string> * args, bool ack)
{
    YAT_INFO << "CommandInterface::command_and_wait: " << cmd.name() << std::endl;
    try
    {
        std::string result;
        m_comms.gpib_write(cmd.str(args), cmd.is_talk_auto());
        cmd.sleep();
        m_comms.gpib_blocking_read(result, timeout, !cmd.is_talk_auto());
        if(ack)
        {
            if(!cmd.is_ack(result))
            {
                THROW_EXCEPTION(
                    "NOT ACKNOWLEDGED",
                    "Received: " + Utils::make_string_readable(result) +
                    " for command " + cmd.name(),
                    "CommandInterface::command_and_wait");
            }
        }
        return result;
    }
    catch(const yat::Exception & ex)
    {
        report_error("CommandInterface::command_and_wait " + cmd.name() + " failed!\n");
        RETHROW_EXCEPTION(ex);
    }
}

std::string CommandInterface::command_and_read(const Command & cmd,
    std::vector<std::string> * args, bool ack)
{
    YAT_INFO << "CommandInterface::command_and_read: " << cmd.name() << std::endl;
    try
    {
        std::string result;
        m_comms.gpib_write(cmd.str(args), cmd.is_talk_auto());
        cmd.sleep();
        m_comms.gpib_read(result, !cmd.is_talk_auto());
        if(ack)
        {
            if(!cmd.is_ack(result))
            {
                THROW_EXCEPTION(
                    "NOT ACKNOWLEDGED",
                    "Received: " + Utils::make_string_readable(result) +
                    " for command " + cmd.name(),
                    "CommandInterface::command_and_read");
            }
        }
        return result;
    }
    catch(const yat::Exception & ex)
    {
        report_error("CommandInterface::command_and_read " + cmd.name() + " failed!\n");
        RETHROW_EXCEPTION(ex);
    }
}

std::string CommandInterface::command_and_flush(const Command & cmd,
    std::vector<std::string> * args, bool ack)
{
    YAT_INFO << "CommandInterface::command_and_flush: " << cmd.name() << std::endl;
    try
    {
        std::string result;
        m_comms.gpib_write(cmd.str(args), cmd.is_talk_auto());
        cmd.sleep();
        m_comms.gpib_flush(result, !cmd.is_talk_auto());
        if(ack)
        {
            if(!cmd.is_ack(result))
            {
                THROW_EXCEPTION(
                    "NOT ACKNOWLEDGED",
                    "Received: " + Utils::make_string_readable(result) +
                    " for command " + cmd.name(),
                    "CommandInterface::command_and_flush");
            }
        }
        return result;
    }
    catch(const yat::Exception & ex)
    {
        report_error("CommandInterface::command_and_flush " + cmd.name() + " failed!\n");
        RETHROW_EXCEPTION(ex);
    }
}


std::string CommandInterface::error_code(int val)
{
    switch (val)
    {
    case 1:
    return "Hardware problem";

    case 2:
    return "Not available";

    case 3:
    return "Parameter problem";

    case 4:
    return "Not initialized";

    case 20:
    return "CCD: NULL user pointer";

    case 21:
    return "CCD: Not enough memory";

    case 22:
    return "CCD: Altparam";

    case 23:
    return "CCD: Load";

    case 24:
    return "CCD: Read program";

    case 25:
    return "CCD: Timeout";

    case 26:
    return"CCD: Zeroloop";

    case 30:
    return "PDA: Multiscan error";

    case 31:
    return "Remote: Not enough memory";

    case 32:
    return "Remote: No data available";

    case 33:
    return "Remote: Binary transfer error";

    case 34:
    return "Remote: Illegal call sequence";

    default:
    return "Unknown error";
    }
}