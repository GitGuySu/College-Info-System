#include<stdio.h>
#include<gtk/gtk.h>
#include<stdlib.h>  
#include<sqlite3.h>


int callback(void *,int, char **, char **);

enum {LIST_TIME, LIST_MON, LIST_TUE, LIST_WED, LIST_THU, LIST_FRI, LIST_SAT, LIST_SUN, N_LABEL};


int main(int argc, char **argv)
{
      GtkWidget *window, *list, *vbox, *label;
      GtkCellRenderer *renderer;
      GtkListStore *store;
      GtkTreeSelection *tree;
      GtkTreeViewColumn *column;

      gtk_init(&argc, &argv);


    store = gtk_list_store_new(N_LABEL, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
      
   sqlite3 *db;
   char *err_msg = 0;

   int rc = sqlite3_open("t1.db", &db);

   if (rc != SQLITE_OK) {

       fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
       sqlite3_close(db);

       return 1;
   }

   char *sql = "DROP TABLE IF EXISTS Timetable;" 
                "CREATE TABLE Timetable(TIME TEXT, MON TEXT, TUE TEXT, WED TEXT, THU TEXT, FRI TEXT, SAT TEXT, SUN TEXT);" 
                "INSERT INTO Timetable VALUES('1-2','a','s','e','f','e','d','f');"
                "INSERT INTO Timetable VALUES('2-3','r','d','d','d','f','f','f');";


    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

   sql = "SELECT * FROM Timetable";

   rc = sqlite3_exec(db, sql, callback, store, &err_msg);

   if (rc != SQLITE_OK ) {

      fprintf(stderr, "Failed to select data\n");
      fprintf(stderr, "SQL error: %s\n", err_msg);

      sqlite3_free(err_msg);
      sqlite3_close(db);

      return 1;
   } 
   else
   {
         printf("Successfully created table\n");
   }
   

   sqlite3_close(db);
      


    //Creating the window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    //Making a list
    list = gtk_tree_view_new();
 

    //Creating Columns
    renderer = gtk_cell_renderer_text_new ();
    column = gtk_tree_view_column_new_with_attributes("TIME",renderer, "text", LIST_TIME, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("MON",
          renderer, "text", LIST_MON, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("TUE",
          renderer, "text", LIST_TUE, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("WED",
          renderer, "text", LIST_WED, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("THU",
          renderer, "text", LIST_THU, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("FRI",
          renderer, "text", LIST_FRI, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("SAT",
          renderer, "text", LIST_SAT, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("SUN",
          renderer, "text", LIST_SUN, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

    gtk_tree_view_set_model(GTK_TREE_VIEW(list), GTK_TREE_MODEL(store));
    g_object_unref(store);

    gtk_window_set_title(GTK_WINDOW(window), "Time Table");
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    gtk_window_set_default_size(GTK_WINDOW(window), 1000, 1000);

    gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(list), FALSE);

    vbox = gtk_box_new(FALSE, 10);

    gtk_box_pack_start(GTK_BOX(vbox), list, TRUE, TRUE, 5);

    label = gtk_label_new("");
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 5);

    gtk_container_add(GTK_CONTAINER(window), vbox);

    g_signal_connect(G_OBJECT (window), "destroy",
    G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}

int callback(void *model, int argc, char **argv, char **azColName)
{
   GtkTreeIter iter;

   for (int i = 0; i < argc; i++)
    {

      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");

    }
    printf("\n");

      //Passing the values into the window

    gtk_list_store_append (GTK_LIST_STORE(model), &iter);
    gtk_list_store_set (GTK_LIST_STORE(model), &iter, LIST_TIME, argv[0],
                                                      LIST_MON, argv[1],
                                                      LIST_TUE, argv[2],
                                                      LIST_WED, argv[3],
                                                      LIST_THU, argv[4],
                                                      LIST_FRI, argv[5],
                                                      LIST_SAT, argv[6],
                                                      LIST_SUN, argv[7],
                                                      -1);


   return 0;
}


