/**********************************************************\

  Auto-generated PowderNPAPIAPI.h

\**********************************************************/

#include <string>
#include <boost/weak_ptr.hpp>

#include "JSAPIAuto.h"
#include "BrowserHost.h"
#include "PowderNPAPI.h"

#ifndef H_PowderNPAPIAPI
#define H_PowderNPAPIAPI

class PowderNPAPIAPI : public FB::JSAPIAuto
{
public:
    ////////////////////////////////////////////////////////////////////////////
    /// @fn PowderNPAPIAPI::PowderNPAPIAPI(const PowderNPAPIPtr& plugin, const FB::BrowserHostPtr host)
    ///
    /// @brief  Constructor for your JSAPI object.
    ///         You should register your methods, properties, and events
    ///         that should be accessible to Javascript from here.
    ///
    /// @see FB::JSAPIAuto::registerMethod
    /// @see FB::JSAPIAuto::registerProperty
    /// @see FB::JSAPIAuto::registerEvent
    ////////////////////////////////////////////////////////////////////////////
    PowderNPAPIAPI(const PowderNPAPIPtr& plugin, const FB::BrowserHostPtr& host) :
        m_plugin(plugin), m_host(host)
    {
        registerMethod("echo",      make_method(this, &PowderNPAPIAPI::echo));
        registerMethod("getStream",      make_method(this, &PowderNPAPIAPI::getStream));
        registerMethod("readStream",      make_method(this, &PowderNPAPIAPI::readStream));
        registerMethod("killStream",      make_method(this, &PowderNPAPIAPI::killStream));
        registerMethod("killAll",      make_method(this, &PowderNPAPIAPI::killAll));
        registerMethod("testEvent", make_method(this, &PowderNPAPIAPI::testEvent));
        
        // Read-write property
        registerProperty("testString",
                         make_property(this,
                                       &PowderNPAPIAPI::get_testString,
                                       &PowderNPAPIAPI::set_testString));
        
        // Read-only property
        registerProperty("version",
                         make_property(this,
                                       &PowderNPAPIAPI::get_version));
    }

    ///////////////////////////////////////////////////////////////////////////////
    /// @fn PowderNPAPIAPI::~PowderNPAPIAPI()
    ///
    /// @brief  Destructor.  Remember that this object will not be released until
    ///         the browser is done with it; this will almost definitely be after
    ///         the plugin is released.
    ///////////////////////////////////////////////////////////////////////////////
    virtual ~PowderNPAPIAPI() {};

    PowderNPAPIPtr getPlugin();

    // Read/Write property ${PROPERTY.ident}
    std::string get_testString();
    void set_testString(const std::string& val);

    // Read-only property ${PROPERTY.ident}
    std::string get_version();

    // Method echo
    FB::variant echo(const FB::variant& msg);
    FB::variant getStream(const std::string& cmdmsg);
    FB::variant readStream(int i);
    FB::variant killStream(int i);
    FB::variant killAll();
    
    // Event helpers
    FB_JSAPI_EVENT(test, 0, ());
    FB_JSAPI_EVENT(echo, 2, (const FB::variant&, const int));
    FB_JSAPI_EVENT(torrentData, 2, (const int, const FB::variant&));

    // Method test-event
    void testEvent();

private:
    PowderNPAPIWeakPtr m_plugin;
    FB::BrowserHostPtr m_host;

    std::string m_testString;
};

#endif // H_PowderNPAPIAPI

