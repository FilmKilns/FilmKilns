
class FkResult {
  final int code;
  final String msg;

  const FkResult(this.code, this.msg);

  static const FkResult OK = FkResult(0, "OK");

  static const FkResult INFO_CAMERA_TAKE_PICTURE_SUCCESS =
      FkResult(1004, "Take picture success");
}
