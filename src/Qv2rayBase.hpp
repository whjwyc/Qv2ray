#ifndef QV2RAYBASE_H
#define QV2RAYBASE_H
#include <QtCore>
#include <QtGui>
#include <QMap>
#include <vector>
#include "QvTinyLog.hpp"
#include "QvCoreConfigObjects.hpp"
#include "QObjectMessageProxy.hpp"

#define QV2RAY_CONFIG_VERSION 12

// Linux DEs should handle the ui schemes themselves.
// --> Or.. should we change this into a modifyable setting?
#ifdef Q_OS_LINUX
#define QV2RAY_USE_BUILTIN_DARKTHEME false
#else
#define QV2RAY_USE_BUILTIN_DARKTHEME true
#endif

// Base folder suffix.
#ifdef QT_DEBUG
#define QV2RAY_CONFIG_DIR_SUFFIX "_debug/"
#else
#define QV2RAY_CONFIG_DIR_SUFFIX "/"
#endif

// Get Configured Config Dir Path
#define QV2RAY_CONFIG_DIR (Qv2ray::Utils::GetConfigDirPath())
#define QV2RAY_CONFIG_FILE (QV2RAY_CONFIG_DIR + "Qv2ray.conf")
#define QV2RAY_QRCODE_DIR (QV2RAY_CONFIG_DIR + "qr_images/")

// Get GFWList and PAC file path.
#define QV2RAY_RULES_DIR (QV2RAY_CONFIG_DIR + "rules/")
#define QV2RAY_RULES_GFWLIST_PATH (QV2RAY_RULES_DIR + "gfwList.txt")
#define QV2RAY_RULES_PAC_PATH (QV2RAY_RULES_DIR + "pac.txt")

#define QV2RAY_CONFIG_FILE_EXTENSION ".qv2ray.json"
#define QV2RAY_GENERATED_DIR (QV2RAY_CONFIG_DIR + "generated/")
#define QV2RAY_GENERATED_FILE_PATH (QV2RAY_GENERATED_DIR + "config.gen.json")

#ifndef QV2RAY_DEFAULT_VCORE_PATH
#ifdef Q_OS_WIN
#define QV2RAY_DEFAULT_VCORE_PATH (QV2RAY_CONFIG_DIR + "vcore/v2ray.exe")
#else
#define QV2RAY_DEFAULT_VCORE_PATH (QV2RAY_CONFIG_DIR + "vcore/v2ray")
#endif
#endif

#define QV2RAY_VCORE_LOG_DIRNAME "logs/"
#define QV2RAY_VCORE_ACCESS_LOG_FILENAME "access.log"
#define QV2RAY_VCORE_ERROR_LOG_FILENAME "error.log"

// GUI TOOLS
#define QV2RAY_IS_DARKTHEME (GetGlobalConfig().uiConfig.useDarkTheme)
#define RED(obj)                               \
    auto _temp = obj->palette();               \
    _temp.setColor(QPalette::Text, Qt::red);   \
    obj->setPalette(_temp);

#define BLACK(obj)                             \
    obj->setPalette(this->palette());

#define QV2RAY_UI_RESOURCES_ROOT QSTRING(QV2RAY_IS_DARKTHEME ? ":/icons/ui_dark/" : ":/icons/ui_light/")
#define QICON_R(file) QIcon(QV2RAY_UI_RESOURCES_ROOT + file)

#define QSTRING(std_string) QString::fromStdString(std_string)
#define NEWLINE "\r\n"

#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

namespace Qv2ray
{
    // Extra header for QvConfigUpgrade.cpp
    QJsonObject UpgradeConfig(int fromVersion, int toVersion, QJsonObject root);

    struct QvBarLine {
        std::string     Family;
        bool            Bold;
        bool            Italic;
        int             ColorA;
        int             ColorR;
        int             ColorG;
        int             ColorB;
        int             ContentType;
        double          Size;
        std::string     Message;
        QvBarLine()
            : Family("Consolas")
            , Bold(true)
            , Italic(false)
            , ColorA(255), ColorR(255), ColorG(255), ColorB(255)
            , ContentType(0)
            , Size(9),
              Message() { }
        XTOSTRUCT(O(Bold, Italic, ColorA, ColorR, ColorG, ColorB, Size, Family, Message, ContentType))
    };

    struct QvBarPage {
        int OffsetYpx;
        vector<QvBarLine> Lines;
        XTOSTRUCT(O(OffsetYpx, Lines))
        QvBarPage() : OffsetYpx(5) { }
    };

    struct Qv2rayToolBarConfig {
        std::vector<QvBarPage> Pages;
        XTOSTRUCT(O(Pages))
    };

    namespace QvConfigModels
    {
        struct Qv2rayPACConfig {
            bool usePAC;
            int port;
            string proxyIP;
            int sourceId;
            bool useSocksProxy;
            string fileLocation;
            Qv2rayPACConfig(): fileLocation() { }
            XTOSTRUCT(O(usePAC, port, proxyIP, sourceId, useSocksProxy, fileLocation))
        };
        struct Qv2rayInboundsConfig {
            string listenip;
            bool setSystemProxy;
            // PAC Config
            Qv2rayPACConfig pacConfig;
            // SOCKS
            int socks_port;
            bool socks_useAuth;
            bool socksUDP;
            string socksLocalIP;
            AccountObject socksAccount;
            // HTTP
            int http_port;
            bool http_useAuth;
            AccountObject httpAccount;
            Qv2rayInboundsConfig(): listenip(), setSystemProxy(), pacConfig(), socks_port(), socks_useAuth(), socksAccount(), http_port(), http_useAuth(), httpAccount() {}
            Qv2rayInboundsConfig(string listen, int socksPort, int httpPort): Qv2rayInboundsConfig()
            {
                socks_port = socksPort;
                http_port = httpPort;
                listenip = listen;
                socksLocalIP = "0.0.0.0";
                socksUDP = true;
                setSystemProxy = true;
            }
            XTOSTRUCT(O(setSystemProxy, pacConfig, listenip, socks_port, socks_useAuth, socksAccount, socksUDP, socksLocalIP, http_port, http_useAuth, httpAccount))
        };

        struct Qv2rayUIConfig {
            string theme;
            string language;
            bool useDarkTheme;
            bool useDarkTrayIcon;
            XTOSTRUCT(O(theme, language, useDarkTheme, useDarkTrayIcon))
        };

        struct Qv2rayConnectionConfig {

            bool bypassCN;
            bool enableProxy;
            bool withLocalDNS;
            list<string> dnsList;
            bool enableStats;
            int statsPort;
            Qv2rayConnectionConfig() : dnsList()
            {
                bypassCN = true;
                enableProxy = true;
                withLocalDNS = true;
            }

            XTOSTRUCT(O(bypassCN, enableProxy, withLocalDNS, dnsList, enableStats, statsPort))

        };
        struct Qv2rayConfig {
            int config_version;
            bool tProxySupport;
            int logLevel;
            //
            string v2CorePath;
            string v2AssetsPath;
            string autoStartConfig;
            //
            string ignoredVersion;
            //
            //
            list<string> configs;
            map<string, string> subscribes;
            //
            Qv2rayUIConfig uiConfig;
            Qv2rayInboundsConfig inboundConfig;
            Qv2rayConnectionConfig connectionConfig;
            Qv2rayToolBarConfig toolBarConfig;

            Qv2rayConfig():
                config_version(QV2RAY_CONFIG_VERSION),
                tProxySupport(false),
                logLevel(),
                v2CorePath(),
                v2AssetsPath(),
                autoStartConfig(),
                ignoredVersion(),
                configs(),
                subscribes(),
                uiConfig(),
                inboundConfig(),
                connectionConfig(),
                toolBarConfig() { }
            //
            Qv2rayConfig(const string &assetsPath, int log, const Qv2rayInboundsConfig &_inBoundSettings): Qv2rayConfig()
            {
                // These settings below are defaults.
                ignoredVersion = "";
                autoStartConfig = "";
                v2AssetsPath = assetsPath;
                inboundConfig = _inBoundSettings;
                logLevel = log;
                tProxySupport = false;
            }
            XTOSTRUCT(O(config_version,
                        ignoredVersion,
                        tProxySupport,
                        logLevel,
                        autoStartConfig,
                        v2CorePath, v2AssetsPath,
                        configs,
                        uiConfig,
                        subscribes, inboundConfig, connectionConfig, toolBarConfig))
        };

    }
}

using namespace std;
using namespace Qv2ray;
using namespace Qv2ray::V2ConfigModels;
using namespace Qv2ray::QvConfigModels;

#endif // QV2RAYBASE_H
