#ifndef GUARD_AITWOYBVCHUJJFBI
#define GUARD_AITWOYBVCHUJJFBI

#include <Eigen/Geometry>

#include <odom_estimator/gravity.h>

namespace odom_estimator {


static const Vec<3> w_E(0, 0, 0.729211510e-4); // from glonass

Vec<3> inertial_from_ecef(double t, Vec<3> ecef_pos) {
  return quat_from_rotvec(w_E * t) * ecef_pos;
}
Vec<3> inertial_vel_from_ecef_vel(double t, Vec<3> ecef_vel, Vec<3> eci_pos) {
  return quat_from_rotvec(w_E * t) * ecef_vel +
    w_E.cross(eci_pos);
}
Quaternion inertial_orient_from_ecef_orient(double t, Quaternion ecef_orient) {
  // (eci<-ecef) * (ecef<-body)
  return quat_from_rotvec(w_E * t) * ecef_orient;
}
Vec<3> inertial_acc_from_ecef_acc(double t, Vec<3> ecef_acc, Vec<3> eci_pos) {
  return quat_from_rotvec(w_E * t) * ecef_acc +
    w_E.cross(w_E.cross(eci_pos));
}

Vec<3> ecef_from_inertial(double t, Vec<3> eci_pos) {
  return quat_from_rotvec(-w_E * t) * eci_pos;
}
Vec<3> ecef_vel_from_inertial_vel(double t, Vec<3> eci_vel, Vec<3> eci_pos) {
  return quat_from_rotvec(-w_E * t) *
    (eci_vel - w_E.cross(eci_pos));
}
Quaternion ecef_orient_from_inertial_orient(double t, Quaternion inertial_orient) {
  return quat_from_rotvec(-w_E * t) * inertial_orient;
}



SqMat<3> enu_from_ecef_mat(Vec<3> zero_pos_ecef) {
  Vec<3> up_ecef = -gravity::gravity(zero_pos_ecef).normalized(); // maybe this should be perpendicular to the ellipsoid instead?
  Vec<3> east_ecef = Vec<3>(0, 0, 1).cross(up_ecef).normalized();
  Vec<3> north_ecef = up_ecef.cross(east_ecef);
  return (SqMat<3>() << east_ecef, north_ecef, up_ecef).finished().transpose();
}

}

#endif
