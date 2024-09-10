#include "base_x.hh";
#include <imgui.h>
#include <romfs/romfs.hpp>
#include <hex/plugin.hpp>
#include <hex/data_processor/node.hpp>
#include <hex/helpers/fmt.hpp>
#include <hex/api/content_registry.hpp>
#include <pl/patterns/pattern.hpp>

namespace {
using namespace hex;

class NodeDecodingBase58 : public dp::Node {
public:
  NodeDecodingBase58() : Node("hex.base58_plugin.nodes.decoding.base58.header", { dp::Attribute(dp::Attribute::IOType::In, dp::Attribute::Type::Buffer, "hex.builtin.nodes.common.input"), dp::Attribute(dp::Attribute::IOType::Out, dp::Attribute::Type::Buffer, "hex.builtin.nodes.common.output") }) { }

  void process() override {
    const auto &input = this->getBufferOnInput(0);
    auto output = Base58::bitcoin().decode(pl::hlp::encodeByteString(input));
    this->setBufferOnOutput(1,  std::vector<uint8_t>(output.begin(), output.end()));
  }
};

void drawBase58InlineVisualizer(pl::ptrn::Pattern &, pl::ptrn::IIterable &, bool, std::span<const pl::core::Token::Literal> arguments) {
  auto decoded = arguments[0].toPattern().get()->getBytes();
  auto encoded = Base58::bitcoin().encode(decoded.data(), decoded.size());
  if (ImGui::Selectable(encoded.c_str(), false, ImGuiSelectableFlags_SelectOnClick, ImVec2(ImGui::GetColumnWidth(), ImGui::GetTextLineHeight()))) {
    ImGui::SetClipboardText(encoded.c_str()); // Copy to clipboard when selected
  }
}
}

IMHEX_PLUGIN_SETUP("Base58 Plugin", "Danny Povolotski", "Adds Base58 support") {
  using ParamCount = pl::api::FunctionParameterCount;

  hex::log::debug("Using romfs: '{}'", romfs::name());
  for (auto &path : romfs::list("lang"))
    hex::ContentRegistry::Language::addLocalization(nlohmann::json::parse(romfs::get(path).string()));

  ContentRegistry::DataProcessorNode::add<NodeDecodingBase58>("hex.builtin.nodes.decoding", "hex.base58_plugin.nodes.decoding.base58");
  ContentRegistry::PatternLanguage::addInlineVisualizer("base58", drawBase58InlineVisualizer, ParamCount::exactly(1));
}