// Copyright © 2017 Loïc Molinari <loicm@loicm.fr>
// Copyright © 2016 Canonical Ltd.
//
// This file is part of Flupke.
//
// Flupke is free software: you can redistribute it and/or modify it under the
// terms of the GNU Lesser General Public License as published by the Free
// Software Foundation; version 3.
//
// Flupke is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with Flupke. If not, see <http://www.gnu.org/licenses/>.

#ifndef APPLICATIONMONITOR_H
#define APPLICATIONMONITOR_H

#include <QtCore/QList>

#include <FlupkeMetrics/logger.h>
#include <FlupkeMetrics/events.h>
#include <FlupkeMetrics/flupkemetricsglobal.h>

class FMApplicationMonitorPrivate;

// Monitor a QtQuick application by automatically tracking QtQuick windows and
// process metrics. The metrics gathered can be logged and displayed by an
// overlay rendered on top of each frame.
class FLUPKE_METRICS_EXPORT FMApplicationMonitor : public QObject
{
    Q_OBJECT

public:
    enum LoggingFilter {
        // Allow process events logging.
        ProcessEvent = (1 << 0),
        // Allow window events logging.
        WindowEvent  = (1 << 1),
        // Allow frame events logging.
        FrameEvent   = (1 << 2),
        // Allow generic events logging.
        GenericEvent = (1 << 3),
        // Allow all events logging.
        AllEvents    = (ProcessEvent | WindowEvent | FrameEvent | GenericEvent)
    };
    Q_DECLARE_FLAGS(LoggingFilters, LoggingFilter)

    enum Event {
        // Application defined event indicating that the initialisation is done
        // and the UI ready. It can be used by tools to measure the time needed
        // to start up an application.
        UserInterfaceReady = 0
    };

    // Get the unique FMApplicationMonitor instance. A QGuiApplication instance
    // must be running.
    static FMApplicationMonitor* instance() { return self ? self : new FMApplicationMonitor; }

    // Render an overlay of real-time metrics on top of each QtQuick frame.
    void setOverlay(bool overlay);
    bool overlay();

    // Log the events with the installed loggers.
    void setLogging(bool logging);
    bool logging();

    // Set the logging filter. All events are logged by default.
    void setLoggingFilter(LoggingFilters filter);
    LoggingFilters loggingFilter();

    // Set the loggers. Empty by default, max number of loggers is 8.
    QList<FMLogger*> loggers();
    bool installLogger(FMLogger* logger);
    bool removeLogger(FMLogger* logger, bool free = true);
    void clearLoggers(bool free = true);

    // Generic event system allowing to log application specific
    // events. registerGenericEvent() returns a unique integer id to be used as
    // first argument to logGenericEvent(). logGenericEvent() logs a generic
    // event with a dedicated id, a null-terminated string describing the event
    // and the string size with the null-terminating character. The maximum
    // string size (with the null-terminating character) is defined in
    // FMGenericEvent::maxStringSize. Does not log and returns false if logging
    // is disabled or if the logging filter does not contain GenericEvent.
    quint32 registerGenericEvent();
    bool logGenericEvent(quint32 id, const char* string, quint32 size);

    // Log events predefined by the application monitor. Relies on the generic
    // event system.
    bool logEvent(Event event);

    // Set the time in milliseconds between two updates of events of a given
    // type. -1 to disable updates. Only FMEvent::Process is accepted so far as
    // event type, default value is 1000. Note that when the overlay is enabled,
    // a process update triggers a frame update.
    void setUpdateInterval(FMEvent::Type type, int interval);
    int updateInterval(FMEvent::Type type);

Q_SIGNALS:
    void overlayChanged();
    void loggingChanged();
    void loggingFilterChanged();
    void loggersChanged();
    void updateIntervalChanged(FMEvent::Type type);

private Q_SLOTS:
    void closeDown();
    void processTimeout();

private:
    static FMApplicationMonitor* self;

    FMApplicationMonitor();
    ~FMApplicationMonitor();
    Q_DISABLE_COPY(FMApplicationMonitor)

    bool eventFilter(QObject* object, QEvent* event) Q_DECL_OVERRIDE;

    FMApplicationMonitorPrivate* const d_ptr;
    Q_DECLARE_PRIVATE(FMApplicationMonitor)
};

#endif  // APPLICATIONMONITOR_H