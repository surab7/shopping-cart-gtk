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
    "Farm fresh eggs, high in protein."
};
const char *item_images[] = {
    "C:/Users/hp5cd/OneDrive/Desktop/Codes/MINI/images/apple.jpg",
    "C:/Users/hp5cd/OneDrive/Desktop/Codes/MINI/images/banana.jpg",
    "C:/Users/hp5cd/OneDrive/Desktop/Codes/MINI/images/milk.jpg",
    "C:/Users/hp5cd/OneDrive/Desktop/Codes/MINI/images/bread.jpg",
    "C:/Users/hp5cd/OneDrive/Desktop/Codes/MINI/images/eggs.jpg"
};
// Function Declarations
void create_main_window(GtkApplication *app);
void create_login_window(GtkApplication *app);
void create_signup_window(GtkApplication *app);
void login(GtkWidget *widget, gpointer data);
void signup(GtkWidget *widget, gpointer data);

// Global Variables
sqlite3 *db;
GtkWidget *login_window, *signup_window;
GtkWidget *username_entry, *password_entry;
GtkWidget *signup_username_entry, *signup_password_entry;

// Database Initialization
void init_db() {
    if (sqlite3_open("shopping_cart.db", &db) != SQLITE_OK) {
        g_print("Failed to open database: %s\n", sqlite3_errmsg(db));
        exit(1);
    }

    // Create Users table if not exists
    const char *sql = "CREATE TABLE IF NOT EXISTS Users (Id INTEGER PRIMARY KEY AUTOINCREMENT, Username TEXT UNIQUE, Password TEXT);";
    char *err_msg = NULL;
    if (sqlite3_exec(db, sql, 0, 0, &err_msg) != SQLITE_OK) {
        g_print("Failed to create Users table: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
}

// Login Function
void login(GtkWidget *widget, gpointer data) {
    const char *username = gtk_entry_get_text(GTK_ENTRY(username_entry));
    const char *password = gtk_entry_get_text(GTK_ENTRY(password_entry));

    if (strlen(username) == 0 || strlen(password) == 0) {
        g_print("Username or Password cannot be empty.\n");
        return;
    }

    char sql[256];
    snprintf(sql, sizeof(sql), "SELECT Id FROM Users WHERE Username='%s' AND Password='%s';", username, password);
    
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            g_print("Login successful!\n");
            gtk_widget_hide(login_window);
            create_main_window(GTK_APPLICATION(data)); // Redirect to main window
        } else {
            g_print("Invalid credentials!\n");
        }
    }
    sqlite3_finalize(stmt);
}

// Signup Function
void signup(GtkWidget *widget, gpointer data) {
    const char *username = gtk_entry_get_text(GTK_ENTRY(signup_username_entry));
    const char *password = gtk_entry_get_text(GTK_ENTRY(signup_password_entry));

    if (strlen(username) == 0 || strlen(password) == 0) {
        g_print("Username or Password cannot be empty.\n");
        return;
    }

    char sql[256];
    snprintf(sql, sizeof(sql), "INSERT INTO Users (Username, Password) VALUES ('%s', '%s');", username, password);
    
    char *err_msg = NULL;
    if (sqlite3_exec(db, sql, 0, 0, &err_msg) == SQLITE_OK) {
        g_print("Signup successful! Please log in.\n");
        gtk_widget_hide(signup_window);
    } else {
        g_print("Signup failed: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
}

// Signup Window Creation
void create_signup_window(GtkApplication *app) {
    signup_window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(signup_window), "Signup");
    gtk_window_set_default_size(GTK_WINDOW(signup_window), 300, 200);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(signup_window), vbox);

    signup_username_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(signup_username_entry), "New Username");
    gtk_box_pack_start(GTK_BOX(vbox), signup_username_entry, FALSE, FALSE, 0);

    signup_password_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(signup_password_entry), "New Password");
    gtk_entry_set_visibility(GTK_ENTRY(signup_password_entry), FALSE);
    gtk_box_pack_start(GTK_BOX(vbox), signup_password_entry, FALSE, FALSE, 0);

    GtkWidget *signup_button = gtk_button_new_with_label("Create Account");
    g_signal_connect(signup_button, "clicked", G_CALLBACK(signup), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), signup_button, FALSE, FALSE, 0);

    gtk_widget_show_all(signup_window);
}

// Login Window Creation (Now with Signup Button)
void create_login_window(GtkApplication *app) {
    login_window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(login_window), "Login");
    gtk_window_set_default_size(GTK_WINDOW(login_window), 300, 250);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(login_window), vbox);

    username_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(username_entry), "Username");
    gtk_box_pack_start(GTK_BOX(vbox), username_entry, FALSE, FALSE, 0);

    password_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(password_entry), "Password");
    gtk_entry_set_visibility(GTK_ENTRY(password_entry), FALSE);
    gtk_box_pack_start(GTK_BOX(vbox), password_entry, FALSE, FALSE, 0);

    GtkWidget *login_button = gtk_button_new_with_label("Login");
    g_signal_connect(login_button, "clicked", G_CALLBACK(login), app);
    gtk_box_pack_start(GTK_BOX(vbox), login_button, FALSE, FALSE, 0);

    // Signup Button
    GtkWidget *signup_button = gtk_button_new_with_label("Signup");
    g_signal_connect(signup_button, "clicked", G_CALLBACK(create_signup_window), app);
    gtk_box_pack_start(GTK_BOX(vbox), signup_button, FALSE, FALSE, 0);

    gtk_widget_show_all(login_window);
}

GtkWidget *quantity_entries[NUM_ITEMS];
int logged_in_user_id = -1;

// Function to add all selected items to the cart
void add_all_to_cart(GtkWidget *widget, gpointer data) {
    for (int i = 0; i < NUM_ITEMS; i++) {
        int quantity = atoi(gtk_entry_get_text(GTK_ENTRY(quantity_entries[i])));
        if (quantity > 0) {
            char sql[256];
            snprintf(sql, sizeof(sql), "INSERT INTO Cart (UserId, Name, Price, Quantity) VALUES (%d, '%s', %.2f, %d);",
                     logged_in_user_id, items[i], prices[i], quantity);
            sqlite3_exec(db, sql, 0, 0, 0);
        }
    }
    g_print("Items added to cart!\n");
}

// Checkout function with QR Code Image
void checkout(GtkWidget *widget, gpointer data) {
    char bill_text[1024] = "==== BILL RECEIPT ====\n";
    float total_price = 0;
    int total_items = 0;

    for (int i = 0; i < NUM_ITEMS; i++) {
        int quantity = atoi(gtk_entry_get_text(GTK_ENTRY(quantity_entries[i])));
        if (quantity > 0) {
            char item_entry[256];
            snprintf(item_entry, sizeof(item_entry), "%s x%d - \u20B9%.2f\n", items[i], quantity, prices[i] * quantity);
            strcat(bill_text, item_entry);
            total_price += prices[i] * quantity;
            total_items += quantity;
        }
    }

    char summary[256];
    snprintf(summary, sizeof(summary), "\nTotal Items: %d\nTotal Price: \u20B9%.2f\nThank you for shopping!", total_items, total_price);
    strcat(bill_text, summary);

    // Create a dialog for the bill
    GtkWidget *dialog = gtk_dialog_new_with_buttons("Payment & Bill",
        GTK_WINDOW(data),
        GTK_DIALOG_DESTROY_WITH_PARENT,
        "_OK", GTK_RESPONSE_OK,
        NULL);

    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *bill_label = gtk_label_new(bill_text);
    gtk_label_set_xalign(GTK_LABEL(bill_label), 0);
    gtk_box_pack_start(GTK_BOX(content_area), bill_label, FALSE, FALSE, 10);

    GtkWidget *qr_image = gtk_image_new_from_file("C:/Users/hp5cd/OneDrive/Desktop/Codes/MINI/images/qr.jpg");
    gtk_box_pack_start(GTK_BOX(content_area), qr_image, FALSE, FALSE, 10);

    gtk_widget_show_all(dialog);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);

    // Save bill to file
    FILE *file = fopen("bill.txt", "w");
    if (file) {
        fprintf(file, "%s\n", bill_text);
        fclose(file);
    }
}

// Create main shopping interface
void create_main_window(GtkApplication *app) {
    GtkWidget *main_window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(main_window), "Shopping Cart");
    gtk_window_set_default_size(GTK_WINDOW(main_window), 800, 600);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(main_window), vbox);

    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_vexpand(scrolled_window, TRUE);
    gtk_box_pack_start(GTK_BOX(vbox), scrolled_window, TRUE, TRUE, 0);

    GtkWidget *flow_box = gtk_flow_box_new();
    gtk_flow_box_set_selection_mode(GTK_FLOW_BOX(flow_box), GTK_SELECTION_NONE);
    gtk_container_add(GTK_CONTAINER(scrolled_window), flow_box);

    for (int i = 0; i < NUM_ITEMS; i++) {
        GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
        GtkWidget *image = gtk_image_new_from_file(item_images[i]);
        gtk_widget_set_size_request(image, 150, 150);
        GtkWidget *label = gtk_label_new(items[i]);
        GtkWidget *price_label = gtk_label_new(g_strdup_printf("\u20B9%.2f", prices[i]));
        quantity_entries[i] = gtk_entry_new();
        gtk_entry_set_placeholder_text(GTK_ENTRY(quantity_entries[i]), "Quantity");
        gtk_widget_set_size_request(quantity_entries[i], 50, -1);
        GtkWidget *desc_label = gtk_label_new(descriptions[i]);
        gtk_label_set_xalign(GTK_LABEL(desc_label), 0);

        gtk_box_pack_start(GTK_BOX(box), image, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(box), price_label, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(box), quantity_entries[i], FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(box), desc_label, FALSE, FALSE, 0);
        gtk_flow_box_insert(GTK_FLOW_BOX(flow_box), box, -1);
    }

    GtkWidget *add_to_cart_button = gtk_button_new_with_label("Add All to Cart");
    gtk_widget_set_size_request(add_to_cart_button, 250, 50);
    g_signal_connect(add_to_cart_button, "clicked", G_CALLBACK(add_all_to_cart), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), add_to_cart_button, FALSE, FALSE, 10);

    GtkWidget *checkout_button = gtk_button_new_with_label("CHECKOUT");
    gtk_widget_set_size_request(checkout_button, 250, 60);
    g_signal_connect(checkout_button, "clicked", G_CALLBACK(checkout), main_window);
    gtk_box_pack_start(GTK_BOX(vbox), checkout_button, FALSE, FALSE, 10);

    gtk_widget_show_all(main_window);
}

int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("com.example.shoppingcart", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(create_login_window), NULL);  // Show login window first

    init_db();
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    sqlite3_close(db);
    return status;
}

