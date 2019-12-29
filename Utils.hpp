#pragma once

#include <algorithm>
#include <cmath>
#include <random>
#include <chrono>

/**
 * Utility class that provide methods for normalizing angles.
 *
 * @author Mathew A. Nelson (original)
 * @author Flemming N. Larsen (contributor)
 */
class Utils
{
	// Hide the default constructor as this class only provides static method
	Utils() {}

public:
    static constexpr double PI = M_PI;
	static constexpr double TWO_PI = 2 * M_PI;
	static constexpr double THREE_PI_OVER_TWO = 3 * M_PI / 2;
	static constexpr double PI_OVER_TWO = M_PI / 2;
	static constexpr double NEAR_DELTA = .00001;

    static constexpr double toRadians = M_PI / 180.;
    static constexpr double toDegrees = 180. / M_PI;

    typedef std::default_random_engine RandomGenerator_t;

	/**
	 * Normalizes an angle to an absolute angle.
	 * The normalized angle will be in the range from 0 to 2*PI, where 2*PI
	 * itself is not included.
	 *
	 * @param angle the angle to normalize
	 * @return the normalized angle that will be in the range of [0,2*PI[
	 */
	static double normalAbsoluteAngle(double angle)
    {
        angle = std::fmod( angle, TWO_PI );
		return angle >= 0 ? angle : (angle + TWO_PI);
	}

	/**
	 * Normalizes an angle to an absolute angle.
	 * The normalized angle will be in the range from 0 to 360, where 360
	 * itself is not included.
	 *
	 * @param angle the angle to normalize
	 * @return the normalized angle that will be in the range of [0,360[
	 */
	static double normalAbsoluteAngleDegrees(double angle)
    {
        angle = std::fmod( angle, 360 );
		return angle >= 0 ? angle : (angle + 360);
	}

	/**
	 * Normalizes an angle to a relative angle.
	 * The normalized angle will be in the range from -PI to PI, where PI
	 * itself is not included.
	 *
	 * @param angle the angle to normalize
	 * @return the normalized angle that will be in the range of [-PI,PI[
	 */
	static double normalRelativeAngle(double angle)
    {
        angle = std::fmod( angle, TWO_PI );
		return angle >= 0 ? (angle < PI) ? angle : angle - TWO_PI : (angle >= -PI) ? angle : angle + TWO_PI;
	}

	/**
	 * Normalizes an angle to a relative angle.
	 * The normalized angle will be in the range from -180 to 180, where 180
	 * itself is not included.
	 *
	 * @param angle the angle to normalize
	 * @return the normalized angle that will be in the range of [-180,180[
	 */
	static double normalRelativeAngleDegrees(double angle)
    {
        angle = std::fmod( angle, 360 );
		return angle >= 0 ? (angle < 180) ? angle : angle - 360 : (angle >= -180) ? angle : angle + 360;
	}

	/**
	 * Normalizes an angle to be near an absolute angle.
	 * The normalized angle will be in the range from 0 to 360, where 360
	 * itself is not included.
	 * If the normalized angle is near to 0, 90, 180, 270 or 360, that
	 * angle will be returned. The {@link #isNear(double, double) isNear}
	 * method is used for defining when the angle is near one of angles listed
	 * above.
	 *
	 * @param angle the angle to normalize
	 * @return the normalized angle that will be in the range of [0,360[
	 * @see #normalAbsoluteAngle(double)
	 * @see #isNear(double, double)
	 */
	static double normalNearAbsoluteAngleDegrees(double angle)
    {
        angle = std::fmod( angle, 360 );
		angle = angle >= 0 ? angle : (angle + 360);

		if (isNear(angle, 180)) {
			return 180;
		} else if (angle < 180) {
			if (isNear(angle, 0)) {
				return 0;
			} else if (isNear(angle, 90)) {
				return 90;
			}
		} else {
			if (isNear(angle, 270)) {
				return 270;
			} else if (isNear(angle, 360)) {
				return 0;
			}
		}
		return angle;
	}

	/**
	 * Normalizes an angle to be near an absolute angle.
	 * The normalized angle will be in the range from 0 to 2*PI, where 2*PI
	 * itself is not included.
	 * If the normalized angle is near to 0, PI/2, PI, 3*PI/2 or 2*PI, that
	 * angle will be returned. The {@link #isNear(double, double) isNear}
	 * method is used for defining when the angle is near one of angles listed
	 * above.
	 *
	 * @param angle the angle to normalize
	 * @return the normalized angle that will be in the range of [0,2*PI[
	 * @see #normalAbsoluteAngle(double)
	 * @see #isNear(double, double)
	 */
	static double normalNearAbsoluteAngle(double angle)
    {
        angle = std::fmod( angle, TWO_PI );
		angle = angle >= 0 ? angle : (angle + TWO_PI);

		if (isNear(angle, PI)) {
			return PI;
		} else if (angle < PI) {
			if (isNear(angle, 0)) {
				return 0;
			} else if (isNear(angle, PI_OVER_TWO)) {
				return PI_OVER_TWO;
			}
		} else {
			if (isNear(angle, THREE_PI_OVER_TWO)) {
				return THREE_PI_OVER_TWO;
			} else if (isNear(angle, TWO_PI)) {
				return 0;
			}
		}
		return angle;
	}

	/**
	 * Tests if the two {@code double} values are near to each other.
	 * It is recommended to use this method instead of testing if the two
	 * doubles are equal using an this expression: {@code value1 == value2}.
	 * The reason being, that this expression might never become
	 * {@code true} due to the precision of double values.
	 * Whether or not the specified doubles are near to each other is defined by
	 * the following expression:
	 * {@code (std::abs(value1 - value2) < .00001)}
	 *
	 * @param value1 the first double value
	 * @param value2 the second double value
	 * @return {@code true} if the two doubles are near to each other;
	 *         {@code false} otherwise.
	 */
	static bool isNear(double value1, double value2)
    {
		return (std::abs(value1 - value2) < NEAR_DELTA);
	}

	/**
	 * Returns random number generator. It might be configured for repeatable behavior by setting -DRANDOMSEED option.
	 *
	 * @return random number generator
	 */
	static RandomGenerator_t& getRandom()
    {
		static unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        static std::default_random_engine generator(seed);
		return generator;
	}

    static double signum( double value )
    {
        return std::copysign( 1., value );
    }
};
