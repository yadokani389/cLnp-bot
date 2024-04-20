#include <dpp/dpp.h>
#include <tic_tac_toe.hpp>

int main() {
  dpp::cluster bot(std::getenv("BOT_TOKEN"), dpp::i_default_intents | dpp::i_message_content);

  bot.on_log(dpp::utility::cout_logger());


  bot.on_slashcommand([](const dpp::slashcommand_t &event) {
    if (event.command.get_command_name() == "ping") {
      event.reply("Pong!");
    }
  });

  bot.on_ready([&bot](const dpp::ready_t &event) {
    if (dpp::run_once<struct register_bot_commands>()) {
      std::vector<dpp::slashcommand> commands;

      commands.push_back(dpp::slashcommand("ping", "pong!", bot.me.id));
      commands.push_back(dpp::slashcommand("tic-tac-toe", "Play tic-tac-toe!", bot.me.id));

      bot.guild_bulk_command_create_sync(commands, 1143218138301861982);
    }
  });

  tic_tac_toe_init(bot);

  bot.start(dpp::st_wait);
}
