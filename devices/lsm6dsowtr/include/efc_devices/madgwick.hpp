//=============================================================================================
// SensorFusion.h
//=============================================================================================
//
// Madgwick's implementation of Mahony's AHRS algorithm.
// See: http://www.x-io.co.uk/open-source-imu-and-ahrs-algorithms/
//
// Date			Author			Notes
// 29/09/2011	SOH Madgwick    Initial release
// 02/10/2011	SOH Madgwick	Optimised for reduced CPU load
// 19/02/2012	SOH Madgwick	Magnetometer measurement is normalised
// 23/11/2017   Aster			Optimised time handling and melted in one library
//
//=============================================================================================
#ifndef SensorFusion_h
#define SensorFusion_h

#include <math.h>
#include <string.h>
#include <cstdint>
#include <esp_timer.h>

//--------------------------------------------------------------------------------------------
// Variable declaration
#define betaDef     0.1f        	// 2 * proportional gain
#define twoKpDef	(2.0f * 0.5f)	// 2 * proportional gain
#define twoKiDef	(2.0f * 0.0f)	// 2 * integral gain

class SF {
//-------------------------------------------------------------------------------------------
// Function declarations

public:
	SF(){
        beta = betaDef;		
        twoKp = twoKpDef;	
        twoKi = twoKiDef;	
        q0 = 1.0f;
        q1 = 0.0f;
        q2 = 0.0f;
        q3 = 0.0f;
        integralFBx = 0.0f;
        integralFBy = 0.0f;
        integralFBz = 0.0f;
        anglesComputed = 0;
    };

    void MadgwickUpdate(float gx, float gy, float gz, float ax, float ay, float az, float deltat) {
       float recipNorm;
        float s0, s1, s2, s3;
        float qDot1, qDot2, qDot3, qDot4;
        float _2q0, _2q1, _2q2, _2q3, _4q0, _4q1, _4q2 ,_8q1, _8q2, q0q0, q1q1, q2q2, q3q3;

        // Rate of change of quaternion from gyroscope
        qDot1 = 0.5f * (-q1 * gx - q2 * gy - q3 * gz);
        qDot2 = 0.5f * (q0 * gx + q2 * gz - q3 * gy);
        qDot3 = 0.5f * (q0 * gy - q1 * gz + q3 * gx);
        qDot4 = 0.5f * (q0 * gz + q1 * gy - q2 * gx);

        // Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
        if(!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {
                // Normalise accelerometer measurement
                recipNorm = invSqrt(ax * ax + ay * ay + az * az);
                ax *= recipNorm;
                ay *= recipNorm;
                az *= recipNorm;

                // Auxiliary variables to avoid repeated arithmetic
                _2q0 = 2.0f * q0;
                _2q1 = 2.0f * q1;
                _2q2 = 2.0f * q2;
                _2q3 = 2.0f * q3;
                _4q0 = 4.0f * q0;
                _4q1 = 4.0f * q1;
                _4q2 = 4.0f * q2;
                _8q1 = 8.0f * q1;
                _8q2 = 8.0f * q2;
                q0q0 = q0 * q0;
                q1q1 = q1 * q1;
                q2q2 = q2 * q2;
                q3q3 = q3 * q3;

                // Gradient decent algorithm corrective step
                s0 = _4q0 * q2q2 + _2q2 * ax + _4q0 * q1q1 - _2q1 * ay;
                s1 = _4q1 * q3q3 - _2q3 * ax + 4.0f * q0q0 * q1 - _2q0 * ay - _4q1 + _8q1 * q1q1 + _8q1 * q2q2 + _4q1 * az;
                s2 = 4.0f * q0q0 * q2 + _2q0 * ax + _4q2 * q3q3 - _2q3 * ay - _4q2 + _8q2 * q1q1 + _8q2 * q2q2 + _4q2 * az;
                s3 = 4.0f * q1q1 * q3 - _2q1 * ax + 4.0f * q2q2 * q3 - _2q2 * ay;
                recipNorm = invSqrt(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3); // normalise step magnitude
                s0 *= recipNorm;
                s1 *= recipNorm;
                s2 *= recipNorm;
                s3 *= recipNorm;

                // Apply feedback step
                qDot1 -= beta * s0;
                qDot2 -= beta * s1;
                qDot3 -= beta * s2;
                qDot4 -= beta * s3;
            }

            // Integrate rate of change of quaternion to yield quaternion
            q0 += qDot1 * deltat;
            q1 += qDot2 * deltat;
            q2 += qDot3 * deltat;
            q3 += qDot4 * deltat;

            // Normalise quaternion
            recipNorm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
            q0 *= recipNorm;
            q1 *= recipNorm;
            q2 *= recipNorm;
            q3 *= recipNorm;
            anglesComputed = 0; 
    }
	
	// find initial Quaternios
	// it is good practice to provide mean values from multiple measurements
    bool initQuat(float ax, float ay, float az, float mx, float my, float mz);
	//these values are already defined by arduino
	const float DEG_TO_RAD = 0.0174532925199433f; //PI/180.0f;	
	const float RAD_TO_DEG = 57.29577951308233f; //180.0f/PI
	
	float deltatUpdate (){
		Now = esp_timer_get_time();
		deltat = ((Now - lastUpdate) / 1000000.0f); // set integration time by time elapsed since last filter update
		lastUpdate = Now;
		return deltat;
	}

	float getRoll() {
		if (!anglesComputed) computeAngles();
		return roll * RAD_TO_DEG;
	}
	float getPitch() {
		if (!anglesComputed) computeAngles();
		return pitch * RAD_TO_DEG;
	}
	float getYaw() {
		if (!anglesComputed) computeAngles();
		return yaw * RAD_TO_DEG + 180.0f;
	}
	float getRollRadians() {
		if (!anglesComputed) computeAngles();
		return roll;
	}
	float getPitchRadians() {
		if (!anglesComputed) computeAngles();
		return pitch;
	}
	float getYawRadians() {
		if (!anglesComputed) computeAngles();
		return yaw;
	}
	float* getQuat() {
		memcpy(_copyQuat, &q0, sizeof(float)*4);
		return _copyQuat;
	}

private:
	float beta;				//Madgwick: 2 * proportional gain
	float twoKp;			//Mahony: 2 * proportional gain (Kp)
	float twoKi;			//Mahony: 2 * integral gain (Ki)
	float q0, q1, q2, q3;	// quaternion of sensor frame relative to auxiliary frame
	float integralFBx, integralFBy, integralFBz;  // integral error terms scaled by Ki
	bool anglesComputed;
	static float invSqrt(float x) {
        float halfx = 0.5f * x;
        union {
        float    f;
        uint32_t i;
        } conv = { .f = x };
        conv.i = 0x5f3759df - (conv.i >> 1);
        conv.f *= 1.5f - (halfx * conv.f * conv.f);
        conv.f *= 1.5f - (halfx * conv.f * conv.f);
        return conv.f;
    };

	void computeAngles() {
        roll = atan2f(q0*q1 + q2*q3, 0.5f - q1*q1 - q2*q2);
        pitch = asinf(-2.0f * (q1*q3 - q0*q2));
        yaw = atan2f(q1*q2 + q0*q3, 0.5f - q2*q2 - q3*q3);
        anglesComputed = 1;
    };

	float roll, pitch, yaw;
	float Now,lastUpdate,deltat;
	float _copyQuat[4];	// copy buffer to protect the quaternion values since getters!=setters
	
};

#endif