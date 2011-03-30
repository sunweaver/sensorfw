/**
   @file clientapitest.h
   @brief Automatic tests for sensor client interfaces

   <p>
   Copyright (C) 2009-2010 Nokia Corporation

   @author Timo Rongas <ext-timo.2.rongas@nokia.com>
   @author Antti Virtanen <antti.i.virtanen@nokia.com>

   This file is part of Sensord.

   Sensord is free software; you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License
   version 2.1 as published by the Free Software Foundation.

   Sensord is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with Sensord.  If not, see <http://www.gnu.org/licenses/>.
   </p>
*/

#ifndef CLIENT_API_TEST_H
#define CLIENT_API_TEST_H

#include <QTest>
#include <QVector>
#include <QStringList>
#include "qt-api/magnetometersensor_i.h"
#include "datatypes/magneticfield.h"
#include "datatypes/xyz.h"

class ClientApiTest : public QObject
{
    Q_OBJECT;

private:
    void calcAverages(QVector<QObject*> data, long& x, long& y,  long& z);
    void calcMaggeAverages(QVector<QObject*> data, long& x, long& y,  long& z, long& rx, long& ry,  long& rz);
    long getLimit(AbstractSensorChannelInterface* sensor);


private slots:
    // Setup tests
    void initTestCase();
    void init();
    void cleanup();
    void cleanupTestCase();

    // Sensors
    void testAccelerometerSensor();
    void testOrientationSensor();
    void testRotationSensor();
    void testTapSensor();
    void testMagnetometerSensor();
    void testCompassSensor();
    void testALSSensor();
    void testProximitySensor();

    // Special cases
    void testCommonAdaptorPipeline();
    void testSessionInitiation();

    // Buffering
    void testBuffering();
    void testBufferingHighRate();
    void testBufferingCompatibility();
    void testBufferingInterval();
    void testAvailableBufferIntervals();
    void testAvailableBufferSizes();

    // Downsampling
    void testDownsampling();
    void testDownsamplingDisabled();


};

class TestClient : public QObject
{
    Q_OBJECT;

public:
    TestClient(AbstractSensorChannelInterface& iface, bool listenFrames);
    virtual ~TestClient();

    int getDataCount() const { return dataCount; }
    int getFrameCount() const { return frameCount; }
    int getFrameDataCount() const { return frameDataCount; }

public Q_SLOTS:
    virtual void dataAvailable(const MagneticField&) { qDebug() << "dataAvailable()"; ++dataCount; }
    virtual void frameAvailable(const QVector<MagneticField>& frame) { qDebug() << "frameAvailable(): " << frame.size(); ++frameCount; frameDataCount += frame.size(); }
    virtual void dataAvailable2(const XYZ&) { qDebug() << "dataAvailable()"; ++dataCount; }
    virtual void frameAvailable2(const QVector<XYZ>& frame) { qDebug() << "frameAvailable(): " << frame.size(); ++frameCount; frameDataCount += frame.size(); }

private:
    int dataCount;
    int frameCount;
    int frameDataCount;
};

class SampleCollector : public TestClient
{
    Q_OBJECT;

public:
    SampleCollector(AbstractSensorChannelInterface& iface, bool listenFrames);
    virtual ~SampleCollector();

//    QVector<MagneticField> getSamples() { return samples; }
    QVector<QObject*> getSamples() { return samples; }

public Q_SLOTS:
    virtual void dataAvailable(const MagneticField&);
    virtual void frameAvailable(const QVector<MagneticField>& frame);
    virtual void dataAvailable2(const XYZ&);
    virtual void frameAvailable2(const QVector<XYZ>& frame);

private:
    QVector<QObject*> samples;
};

#endif // CLIENT_API_TEST_H
