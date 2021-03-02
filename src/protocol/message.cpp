#include "protocol/message.h"
#include "base/json/json.h"

constexpr char kMessageType[] = "type";
constexpr char kDeviceName[] = "device_name";
constexpr char kDeviceId[] = "from_id";

std::string LoginMessage::SerializeAsJsonString() {
  base::Json json;
  json.AddMember(kMessageType, (int)type_);
  json.AddMember(kDeviceName, device_name_);

  return json.Str();
}

std::string LoginResponseMessage::SerializeAsJsonString() {
  base::Json json;
  json.AddMember(kMessageType, (int)type_);
  json.AddMember(kDeviceId, device_id_);
  return json.Str();
}


std::string OfferMessage::SerializeAsJsonString() {
  base::Json json;

  return json.Str();
}

std::string AnswerMessage::SerializeAsJsonString() {
  base::Json json;

  return json.Str();
}

std::string CandidateMessage::SerializeAsJsonString() {
  base::Json json;

  return json.Str();
}

std::shared_ptr<Message> Message::ParseMessage(const std::string& json_str) {
  return nullptr;
}