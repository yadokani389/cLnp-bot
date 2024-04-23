#include <dpp/dpp.h>

#include <tic_tac_toe.hpp>

int main() {
  const char *BOT_TOEKN_PTR = std::getenv("BOT_TOKEN");
  if (!BOT_TOEKN_PTR) {
    std::cout << "Set enviroment viriable BOT_TOKEN.\n";
    return 0;
  }
  const std::string BOT_TOKEN(BOT_TOEKN_PTR);
  if (BOT_TOKEN.empty()) {
    std::cout << "Set enviroment viriable BOT_TOKEN.\n";
    return 0;
  }

  dpp::cluster bot(BOT_TOKEN, dpp::i_default_intents | dpp::i_message_content);

  bot.on_log(dpp::utility::cout_logger());

  bot.on_slashcommand([](const dpp::slashcommand_t &event) {
    if (event.command.get_command_name() == "ping") {
      auto embed = dpp::embed()
                       .set_title("embed")
                       .add_field("name", "<t:1713706320:d>");
      dpp::message msg(event.command.channel_id, embed);
      event.reply(msg);
    }
  });

  bot.on_ready([&bot](const dpp::ready_t &event) {
    if (dpp::run_once<struct register_bot_commands>()) {
      std::vector<dpp::slashcommand> commands;

      commands.push_back(dpp::slashcommand("ping", "pong!", bot.me.id));
      commands.push_back(dpp::slashcommand("tic-tac-toe", "Play tic-tac-toe!", bot.me.id));

      bot.guild_bulk_command_create(commands, 1143218138301861982);
    }
  });

  tic_tac_toe_init(bot);

  bot.start(dpp::st_wait);
}
