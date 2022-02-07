/* hid-gadget-keyboard

Modified from https://www.kernel.org/doc/html/latest/usb/gadget_hid.html
*/

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUF_LEN 512

struct options
{
    const char *opt;
    unsigned char val;
};

// https://www.usb.org/document-library/hid-usage-tables-122

static struct options kmod[] = {
    {.opt = "--left-ctrl", .val = 0x01},
    {.opt = "--right-ctrl", .val = 0x10},
    {.opt = "--left-shift", .val = 0x02},
    {.opt = "--right-shift", .val = 0x20},
    {.opt = "--left-alt", .val = 0x04},
    {.opt = "--right-alt", .val = 0x40},
    {.opt = "--left-meta", .val = 0x08},
    {.opt = "--right-meta", .val = 0x80},
    {.opt = NULL}};

static struct options kval[] = {
    {.opt = "--return", .val = 0x28},
    {.opt = "--enter", .val = 0x28},
    {.opt = "--esc", .val = 0x29},
    {.opt = "--bckspc", .val = 0x2a},
    {.opt = "--tab", .val = 0x2b},
    {.opt = "--spacebar", .val = 0x2c},
    {.opt = "--caps-lock", .val = 0x39},
    {.opt = "--f1", .val = 0x3a},
    {.opt = "--f2", .val = 0x3b},
    {.opt = "--f3", .val = 0x3c},
    {.opt = "--f4", .val = 0x3d},
    {.opt = "--f5", .val = 0x3e},
    {.opt = "--f6", .val = 0x3f},
    {.opt = "--f7", .val = 0x40},
    {.opt = "--f8", .val = 0x41},
    {.opt = "--f9", .val = 0x42},
    {.opt = "--f10", .val = 0x43},
    {.opt = "--f11", .val = 0x44},
    {.opt = "--f12", .val = 0x45},
    {.opt = "--insert", .val = 0x49},
    {.opt = "--home", .val = 0x4a},
    {.opt = "--pageup", .val = 0x4b},
    {.opt = "--del", .val = 0x4c},
    {.opt = "--end", .val = 0x4d},
    {.opt = "--pagedown", .val = 0x4e},
    {.opt = "--right", .val = 0x4f},
    {.opt = "--left", .val = 0x50},
    {.opt = "--down", .val = 0x51},
    {.opt = "--kp-enter", .val = 0x58},
    {.opt = "--up", .val = 0x52},
    {.opt = "--num-lock", .val = 0x53},
    {.opt = NULL}};

int keyboard_fill_report(char report[8], char buf[BUF_LEN], int *hold)
{
    char *tok = strtok(buf, " ");
    int key = 0;
    int i = 0;

    for (; tok != NULL; tok = strtok(NULL, " "))
    {

        if (strcmp(tok, "--quit") == 0)
            return -1;

        if (strcmp(tok, "--hold") == 0)
        {
            *hold = 1;
            continue;
        }

        if (key < 6)
        {
            for (i = 0; kval[i].opt != NULL; i++)
                if (strcmp(tok, kval[i].opt) == 0)
                {
                    report[2 + key++] = kval[i].val;
                    break;
                }

            if (kval[i].opt != NULL)
                continue;
        }

        if (key < 6)
        {
            if (islower(tok[0]))
            {
                report[2 + key++] = (tok[0] - ('a' - 0x04));
                continue;
            }
        }

        for (i = 0; kmod[i].opt != NULL; i++)
        {
            if (strcmp(tok, kmod[i].opt) == 0)
            {
                report[0] = report[0] | kmod[i].val;
                break;
            }
        }

        if (kmod[i].opt != NULL)
            continue;

        if (key < 6)
            fprintf(stderr, "unknown option: %s\n", tok);
    }
    return 8;
}

void print_options()
{
    int i = 0;
    printf("        keyboard options:\n"
           "                --hold\n");
    for (i = 0; kmod[i].opt != NULL; i++)
        printf("\t\t%s\n", kmod[i].opt);
    printf("\n        keyboard values:\n"
           "                [a-z] or\n");
    for (i = 0; kval[i].opt != NULL; i++)
        printf("\t\t%-8s%s", kval[i].opt, i % 2 ? "\n" : "");
    printf("\n");
}

int main(int argc, const char *argv[])
{
    const char *filename = NULL;
    int fd = 0;
    char buf[BUF_LEN];
    int cmd_len;
    char report[8];
    int to_send = 8;
    int hold = 0;
    int delay = 0;
    fd_set rfds;
    int retval, i;

    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <devname> [delay]\n", argv[0]);
        print_options();
        return 1;
    }

    filename = argv[1];

    if (argc > 2)
        delay = atoi(argv[2]);

    if ((fd = open(filename, O_RDWR, 0666)) == -1)
    {
        perror(filename);
        return 3;
    }

    int counter = 0;

    while (42)
    {
        counter++;
        FD_ZERO(&rfds);
        FD_SET(STDIN_FILENO, &rfds);
        FD_SET(fd, &rfds);

        retval = select(fd + 1, &rfds, NULL, NULL, NULL);
        if (retval == -1 && errno == EINTR)
            continue;
        if (retval < 0)
        {
            perror("select()");
            return 4;
        }

        if (FD_ISSET(fd, &rfds))
        {
            cmd_len = read(fd, buf, BUF_LEN - 1);
            printf("recv report:");
            for (i = 0; i < cmd_len; i++)
                printf(" %02x", buf[i]);
            printf("\n");
        }

        if (FD_ISSET(STDIN_FILENO, &rfds))
        {
            memset(report, 0x0, sizeof(report));
            cmd_len = read(STDIN_FILENO, buf, BUF_LEN - 1);

            if (cmd_len == 0)
                break;

            buf[cmd_len - 1] = '\0';
            hold = 0;

            memset(report, 0x0, sizeof(report));
            to_send = keyboard_fill_report(report, buf, &hold);

            if (to_send == -1)
                break;

            if (write(fd, report, to_send) != to_send)
            {
                perror(filename);
                return 5;
            }

            if (!hold)
            {
                // Delay before releasing the key(s)
                if (delay > 0)
                    usleep(delay * 1000);

                memset(report, 0x0, sizeof(report));
                if (write(fd, report, to_send) != to_send)
                {
                    perror(filename);
                    return 6;
                }
            }
        }
    }

    close(fd);
    return 0;
}
