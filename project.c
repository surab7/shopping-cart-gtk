#include <gtk/gtk.h>
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_ITEMS 5

const char *items[] = {"Apple", "Banana", "Milk", "Bread", "Eggs"};
const float prices[] = {60, 25, 80, 40, 6.0};
const char *descriptions[] = {
    "Fresh red apples, rich in fiber.",
    "Organic bananas, full of potassium.",
    "Nutritious milk, great for health.",
    "Soft and fresh bread for breakfast.",
    "Farm fresh eggs, high in protein."};
const char *item_images[] = {
    "C:/Users/hp5cd/Desktop/Codes/MINI/images/apple.jpg",
    "C:/Users/hp5cd/Desktop/Codes/MINI/images/banana.jpg",
    "C:/Users/hp5cd/Desktop/Codes/MINI/images/milk.jpg",
    "C:/Users/hp5cd/Desktop/Codes/MINI/images/bread.jpg",
    "C:/Users/hp5cd/Desktop/Codes/MINI/images/eggs.jpg"};

sqlite3 *db;
GtkWidget *login_window, *signup_window;
GtkWidget *username_entry, *password_entry;
GtkWidget *signup_username_entry, *signup_password_entry;

GtkWidget *quantity_labels[NUM_ITEMS];
int quantities[NUM_ITEMS] = {0};

void create_main_window(GtkApplication *app);
void create_login_window(GtkApplication *app);
void create_signup_window(GtkApplication *app);
void login(GtkWidget *widget, gpointer data);
void signup(GtkWidget *widget, gpointer data);

void open_signup(GtkWidget *widget, gpointer data)
{
    create_signup_window(GTK_APPLICATION(data));
}

void init_db()
{
    if (sqlite3_open("shopping_cart.db", &db) != SQLITE_OK)
        exit(1);

    sqlite3_exec(db,
                 "CREATE TABLE IF NOT EXISTS Users (Id INTEGER PRIMARY KEY AUTOINCREMENT, Username TEXT UNIQUE, Password TEXT);",
                 0, 0, 0);
}

void login(GtkWidget *widget, gpointer data)
{
    const char *username = gtk_entry_get_text(GTK_ENTRY(username_entry));
    const char *password = gtk_entry_get_text(GTK_ENTRY(password_entry));

    char sql[256];
    snprintf(sql, sizeof(sql),
             "SELECT Id FROM Users WHERE Username='%s' AND Password='%s';",
             username, password);

    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        gtk_widget_hide(login_window);
        create_main_window(GTK_APPLICATION(data));
    }
    sqlite3_finalize(stmt);
}

void signup(GtkWidget *widget, gpointer data)
{
    const char *username = gtk_entry_get_text(GTK_ENTRY(signup_username_entry));
    const char *password = gtk_entry_get_text(GTK_ENTRY(signup_password_entry));

    char sql[256];
    snprintf(sql, sizeof(sql),
             "INSERT INTO Users (Username, Password) VALUES ('%s','%s');",
             username, password);

    sqlite3_exec(db, sql, 0, 0, 0);
    gtk_widget_hide(signup_window);
}

void create_signup_window(GtkApplication *app)
{
    signup_window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(signup_window), "Signup");
    gtk_window_set_default_size(GTK_WINDOW(signup_window), 300, 200);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(signup_window), vbox);

    signup_username_entry = gtk_entry_new();
    signup_password_entry = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(signup_password_entry), FALSE);

    GtkWidget *btn = gtk_button_new_with_label("Create Account");
    g_signal_connect(btn, "clicked", G_CALLBACK(signup), NULL);

    gtk_box_pack_start(GTK_BOX(vbox), signup_username_entry, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), signup_password_entry, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), btn, FALSE, FALSE, 0);

    gtk_widget_show_all(signup_window);
}

void create_login_window(GtkApplication *app)
{
    login_window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(login_window), "Login");
    gtk_window_set_default_size(GTK_WINDOW(login_window), 300, 250);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(login_window), vbox);

    username_entry = gtk_entry_new();
    password_entry = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(password_entry), FALSE);

    GtkWidget *login_btn = gtk_button_new_with_label("Login");
    GtkWidget *signup_btn = gtk_button_new_with_label("Signup");

    g_signal_connect(login_btn, "clicked", G_CALLBACK(login), app);
    g_signal_connect(signup_btn, "clicked", G_CALLBACK(open_signup), app);

    gtk_box_pack_start(GTK_BOX(vbox), username_entry, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), password_entry, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), login_btn, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), signup_btn, FALSE, FALSE, 0);

    gtk_widget_show_all(login_window);
}

void increase_quantity(GtkWidget *w, gpointer d)
{
    int i = GPOINTER_TO_INT(d);
    quantities[i]++;
    char buf[10];
    sprintf(buf, "%d", quantities[i]);
    gtk_label_set_text(GTK_LABEL(quantity_labels[i]), buf);
}

void decrease_quantity(GtkWidget *w, gpointer d)
{
    int i = GPOINTER_TO_INT(d);
    if (quantities[i] > 0)
        quantities[i]--;
    char buf[10];
    sprintf(buf, "%d", quantities[i]);
    gtk_label_set_text(GTK_LABEL(quantity_labels[i]), buf);
}

void add_all_to_cart(GtkWidget *widget, gpointer data)
{
    for (int i = 0; i < NUM_ITEMS; i++)
        if (quantities[i] > 0)
            g_print("%s x%d\n", items[i], quantities[i]);
}

void checkout(GtkWidget *widget, gpointer data)
{
    char bill_text[1024] = "==== BILL RECEIPT ====\n";
    float total_price = 0;
    int total_items = 0;

    for (int i = 0; i < NUM_ITEMS; i++)
    {
        if (quantities[i] > 0)
        {
            char line[256];
            snprintf(line, sizeof(line),
                     "%s x%d - ₹%.2f\n",
                     items[i], quantities[i], prices[i] * quantities[i]);
            strcat(bill_text, line);
            total_price += prices[i] * quantities[i];
            total_items += quantities[i];
        }
    }

    char summary[256];
    snprintf(summary, sizeof(summary),
             "\nTotal Items: %d\nTotal Price: ₹%.2f\nThank you for shopping!",
             total_items, total_price);
    strcat(bill_text, summary);

    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Payment & Bill",
        GTK_WINDOW(data),
        GTK_DIALOG_DESTROY_WITH_PARENT,
        "_OK",
        GTK_RESPONSE_OK,
        NULL);

    GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *label = gtk_label_new(bill_text);
    gtk_label_set_xalign(GTK_LABEL(label), 0);
    gtk_box_pack_start(GTK_BOX(content), label, FALSE, FALSE, 10);

    GtkWidget *qr = gtk_image_new_from_file(
        "C:/Users/hp5cd/Desktop/Codes/MINI/images/qr.jpg");
    gtk_box_pack_start(GTK_BOX(content), qr, FALSE, FALSE, 10);

    gtk_widget_show_all(dialog);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void create_main_window(GtkApplication *app)
{
    GtkWidget *main_window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(main_window), "Shopping Cart");
    gtk_window_set_default_size(GTK_WINDOW(main_window), 800, 600);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(main_window), vbox);

    GtkWidget *scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_box_pack_start(GTK_BOX(vbox), scroll, TRUE, TRUE, 0);

    GtkWidget *flow = gtk_flow_box_new();
    gtk_flow_box_set_selection_mode(GTK_FLOW_BOX(flow), GTK_SELECTION_NONE);
    gtk_flow_box_set_activate_on_single_click(GTK_FLOW_BOX(flow), FALSE);
    gtk_container_add(GTK_CONTAINER(scroll), flow);

    for (int i = 0; i < NUM_ITEMS; i++)
    {
        GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
        GtkWidget *img = gtk_image_new_from_file(item_images[i]);
        gtk_widget_set_size_request(img, 150, 150);

        GtkWidget *name = gtk_label_new(items[i]);
        GtkWidget *price = gtk_label_new(g_strdup_printf("₹%.2f", prices[i]));
        GtkWidget *desc = gtk_label_new(descriptions[i]);

        gtk_label_set_xalign(GTK_LABEL(desc), 0.5); // center horizontally
        gtk_label_set_yalign(GTK_LABEL(desc), 0.0); // top align vertically
        gtk_label_set_line_wrap(GTK_LABEL(desc), TRUE);
        gtk_label_set_max_width_chars(GTK_LABEL(desc), 24);
        gtk_label_set_justify(GTK_LABEL(desc), GTK_JUSTIFY_CENTER);

        gtk_widget_set_margin_top(desc, 6);
        gtk_widget_set_margin_bottom(desc, 4);

        GtkWidget *qty_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
        gtk_widget_set_halign(qty_box, GTK_ALIGN_CENTER);

        GtkWidget *minus = gtk_button_new_with_label("-");
        GtkWidget *plus = gtk_button_new_with_label("+");

        quantity_labels[i] = gtk_label_new("0");
        gtk_label_set_xalign(GTK_LABEL(quantity_labels[i]), 0.5);

        g_signal_connect(minus, "clicked", G_CALLBACK(decrease_quantity), GINT_TO_POINTER(i));
        g_signal_connect(plus, "clicked", G_CALLBACK(increase_quantity), GINT_TO_POINTER(i));

        gtk_box_pack_start(GTK_BOX(qty_box), minus, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(qty_box), quantity_labels[i], FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(qty_box), plus, FALSE, FALSE, 0);

        gtk_box_pack_start(GTK_BOX(box), img, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(box), name, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(box), price, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(box), desc, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(box), qty_box, FALSE, FALSE, 0);

        gtk_flow_box_insert(GTK_FLOW_BOX(flow), box, -1);
    }

    GtkWidget *add_btn = gtk_button_new_with_label("Add All to Cart");
    GtkWidget *checkout_btn = gtk_button_new_with_label("CHECKOUT");

    g_signal_connect(add_btn, "clicked", G_CALLBACK(add_all_to_cart), NULL);
    g_signal_connect(checkout_btn, "clicked", G_CALLBACK(checkout), main_window);

    gtk_box_pack_start(GTK_BOX(vbox), add_btn, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(vbox), checkout_btn, FALSE, FALSE, 10);

    gtk_widget_show_all(main_window);
}

void load_css()
{
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(provider, "style.css", NULL);
    gtk_style_context_add_provider_for_screen(
        gdk_screen_get_default(),
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

int main(int argc, char **argv)
{
    GtkApplication *app =
        gtk_application_new("com.example.shoppingcart", G_APPLICATION_DEFAULT_FLAGS);

    g_signal_connect(app, "activate", G_CALLBACK(create_login_window), NULL);

    load_css();

    init_db();
    int status = g_application_run(G_APPLICATION(app), argc, argv);

    g_object_unref(app);
    sqlite3_close(db);
    return status;
}
