#include "tic_tac_toe.hpp"

void tic_tac_toe_init(dpp::cluster &bot) {
  static std::map<dpp::snowflake, std::array<char, 9>> tic_tac_toe_map;

  bot.on_slashcommand([&bot](const dpp::slashcommand_t &event) {
    if (event.command.get_command_name() == "tic-tac-toe") {
      auto &field = tic_tac_toe_map[event.command.channel_id];
      field.fill('\0');

      dpp::message msg = dpp::message(event.command.channel_id, "");
      if (std::count(field.begin(), field.end(), 'X') < std::count(field.begin(), field.end(), 'O'))
        msg.content = "X's turn";
      else
        msg.content = "O's turn";

      for (size_t i = 0; i < 3; i++) {
        dpp::component cmp;
        for (size_t j = 0; j < 3; j++) {
          cmp.add_component(
              dpp::component()
                  .set_label(std::string(1, field[j + i * 3]))
                  .set_style(dpp::cos_primary)
                  .set_disabled(field[j + i * 3])
                  .set_id("tic-tac-toe" + std::string(1, i + '0') + std::string(1, j + '0')));
        }
        msg.add_component(cmp);
      }

      event.reply(msg);
    }
  });

  bot.on_button_click([&bot](const dpp::button_click_t &event) {
    if (event.custom_id.starts_with("tic-tac-toe")) {
      auto &field = tic_tac_toe_map[event.command.channel_id];
      auto msg = dpp::message(event.command.channel_id, "");
      msg.id = event.command.message_id;

      char pre_turn;
      if (std::count(field.begin(), field.end(), 'X') < std::count(field.begin(), field.end(), 'O')) {
        msg.content = "O's turn";
        pre_turn = 'X';
      } else {
        msg.content = "X's turn";
        pre_turn = 'O';
      }

      size_t y = event.custom_id[event.custom_id.size() - 1] - '0';
      size_t x = event.custom_id[event.custom_id.size() - 2] - '0';

      for (size_t i = 0; i < 3; i++) {
        dpp::component cmp;
        for (size_t j = 0; j < 3; j++) {
          if (x == i && y == j)
            field[j + i * 3] = pre_turn;
          cmp.add_component(
              dpp::component()
                  .set_label(std::string(1, field[j + i * 3]))
                  .set_style(dpp::cos_primary)
                  .set_disabled(field[j + i * 3])
                  .set_id("tic-tac-toe" + std::string(1, i + '0') + std::string(1, j + '0')));
        }
        msg.add_component(cmp);
      }

      {
        char winner = '\0';
        for (int i = 0; i < 3; i++) {
          if (field[0 + i * 3] == field[1 + i * 3] && field[1 + i * 3] == field[2 + i * 3])
            winner = field[0 + i * 3];
          if (field[i + 0 * 3] == field[i + 1 * 3] && field[i + 1 * 3] == field[i + 2 * 3])
            winner = field[i + 0 * 3];
        }
        if (field[0 + 0 * 3] == field[1 + 1 * 3] && field[1 + 1 * 3] == field[2 + 2 * 3])
          winner = field[0 + 0 * 3];
        if (field[2 + 0 * 3] == field[1 + 1 * 3] && field[1 + 1 * 3] == field[0 + 2 * 3])
          winner = field[2 + 0 * 3];

        if (winner) {
          msg.content = std::string(1, winner) + " wins!";
          bot.message_edit_sync(msg);
          event.reply();
          field.fill('\0');

          return;
        }
      }

      if (std::count(field.begin(), field.end(), '\0') == 0) {
        msg.content = "draw!";
        bot.message_edit_sync(msg);
        event.reply();
        field.fill('\0');

        return;
      }

      bot.message_edit_sync(msg);
      event.reply();
    }
  });
}
