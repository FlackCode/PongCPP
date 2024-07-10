#define is_down(b) input->buttons[b].is_down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)

float playerPosY = 0.f;
float playerPosX = 0.f;

internal void simulate_game(Input* input, float dt) {
    clear_screen(0xffffff);
    float speed = 50.f;
    if (is_down(BUTTON_UP))
        playerPosY += speed*dt;
    if(is_down(BUTTON_DOWN))
        playerPosY -= speed * dt;
    if(is_down(BUTTON_LEFT))
        playerPosX -= speed * dt;
    if (is_down(BUTTON_RIGHT))
        playerPosX += speed * dt;
    draw_rect(playerPosX, playerPosY, 10, 10, 0xff0000);
    draw_rect(30, 30, 5, 5, 0xff0000);
    draw_rect(-20, 20, 8, 3, 0xff0000);
}