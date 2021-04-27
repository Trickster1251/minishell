int g_sig = 0;
int g_error = 0; $?
int g_quit = 0;
void    sigint_operate(int key)
{
    (void)key;
    if (g_sig == 0)
    {
        write(1, "\b\b  \b\b", 5);
        write(1, "\nminishell-0.1$ ", 16);
    }
    else if (g_sig == 1)
    {
        g_error = 130;
        write(1, "\n", 1);
    }
}
void    quit_operate(int key)
{
    if (g_quit != 0 && g_quit != 2)
    {
        write(1, "Quit: ", 6);
        ft_putnbr_fd(key, 1);
        write(1, "\n", 1);
    }
    else if (g_quit == 0)
        write(1, "\b\b  \b\b", 6);
    if (g_quit != 0 && g_quit != 2)
        g_error = 131;
}
int     main(int argc, char **argv, char **envp)
{
    t_col       col;
    (void)argc;
    (void)envp;
    signal(SIGINT, sigint_operate);
    signal(SIGQUIT, quit_operate);
    initstruct(&col, envp);
    starter(argc, argv, &col);
    return (0);
}