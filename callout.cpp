
//------------------------------------------------------------------------------
//These includes are needed for the following template code
//------------------------------------------------------------------------------
#include "callout.hpp"

using namespace NXOpen;
using namespace NXOpen::BlockStyler;

tag_t view;
double abs_cursor_pos[3];

typedef struct user_data_s 
{
    int max_count;
} user_data_t, * user_data_p_t;



typedef struct
{
    UF_CURVE_arc_t* arc;
    double          abs_ctr[3];
    double          matrix[9];
    double          x_axis[3];
    double          y_axis[3];
} my_motion_data_t;




double size_note = 3;
int const count_note = 3;
//char note_text[count_note][MAX_LINE_BUFSIZE];
std::string str_text[count_note];
//tag_t symbol_tags[count_note];
double coord_line[3];
double coord_notes[3];
double coord_symbol[3];



void test();
void test1();
static void create_custom_symbol(void);
static void create_symbol_leader(UF_DRF_custom_symbol_p_t symbol_definition);


void do_ugopen_api(void);
static void do_ugopen_api1(void);
static int init_proc( UF_UI_selection_p_t select, void* user_data);
static int sel_cb (
        int num_selected, tag_p_t objects, int num_deselected,
        tag_p_t deselected_objects, void* user_data, UF_UI_selection_p_t select);

static void create_callout(double coordX, double coordY, my_motion_data_t* my_data);

void create_callout(double coor[], my_motion_data_t* my_data, NXOpen::BlockStyler::SpecifyPoint* point0);

/* Define a motion callback to render a visual representation of
 * the circle to be created, along with a bounding box around the
 * circle, an arc of radius 1.0, and a "rubberband" line from the
 * center to the cursor position.
 */
static void motion_cb(double* screen_pos,
    UF_UI_motion_cb_data_p_t  motion_cb_data,
    my_motion_data_t* my_data)
{
    double radius, pos_array[5][3];
    double xrad[3], yrad[3], x_y_vec[3], x_ny_vec[3];

    UF_OBJ_disp_props_t attrib;

    //double coord_line[3];
    //double coord_notes[3];
    //double coord_symbol[3];

    if (screen_pos[0] < my_data->abs_ctr[0])
    {
        coord_line[0] = screen_pos[0];
        coord_line[1] = screen_pos[1] - (10 * (count_note - 1));
        coord_line[2] = screen_pos[2];
        coord_notes[0] = screen_pos[0] - 10;
        coord_notes[1] = screen_pos[1];
        coord_notes[2] = screen_pos[2];
        coord_symbol[0] = screen_pos[0]-10;
        coord_symbol[1] = screen_pos[1]+2;
        coord_symbol[2] = screen_pos[2];
    }
    else
    {
        //double coord_line[3] = { screen_pos[0], screen_pos[1] - (10 * (count_note - 1)), screen_pos[2] };
       //double coord_notes[3] = { screen_pos[0] - 10, screen_pos[1], screen_pos[2] };
       // double coord_symbol[3] = { screen_pos[0] + 2, screen_pos[1] + 2, screen_pos[2] };
        coord_line[0] = screen_pos[0];
        coord_line[1] = screen_pos[1] - (10 * (count_note - 1));
        coord_line[2] = screen_pos[2];
        coord_notes[0] = screen_pos[0] + 10;
        coord_notes[1] = screen_pos[1];
        coord_notes[2] = screen_pos[2];
        coord_symbol[0] = screen_pos[0]+2;
        coord_symbol[1] = screen_pos[1]+2;
        coord_symbol[2] = screen_pos[2];
    }
    UF_DISP_refresh();
    UF_OBJ_ask_display_properties(motion_cb_data->view_tag, &attrib);
    attrib.color = 148;
    // Calculate the arc radius:  the distance from the arc center to the current screen position.
     
   /* UF_VEC3_distance(my_data->abs_ctr, screen_pos, &radius);
    // Map the arc center to the "csys of the arc".

    UF_MTX3_vec_multiply(my_data->abs_ctr, my_data->matrix, my_data->arc->arc_center);

    // Draw a circle and an arc in the view of the cursor.
    UF_DISP_display_ogp_circle(motion_cb_data->view_tag, my_data->matrix,
        my_data->arc->arc_center, radius);

    UF_DISP_display_ogp_arc(motion_cb_data->view_tag,
        my_data->matrix, 15.0 * DEGRA, 345.0 * DEGRA,
        my_data->arc->arc_center, 1.0);

    // Draw a bounding box around the circle.
    UF_VEC3_scale(radius, my_data->x_axis, xrad);
    UF_VEC3_scale(radius, my_data->y_axis, yrad);

    UF_VEC3_add(xrad, yrad, x_y_vec);
    UF_VEC3_sub(xrad, yrad, x_ny_vec);

    UF_VEC3_add(my_data->abs_ctr, x_y_vec, pos_array[0]);
    UF_VEC3_sub(my_data->abs_ctr, x_ny_vec, pos_array[1]);
    UF_VEC3_sub(my_data->abs_ctr, x_y_vec, pos_array[2]);
    UF_VEC3_add(my_data->abs_ctr, x_ny_vec, pos_array[3]);
    UF_VEC3_add(my_data->abs_ctr, x_y_vec, pos_array[4]);
    UF_DISP_display_ogp_polyline(motion_cb_data->view_tag, pos_array, 5);*/

    // Draw a "rubberband" line from the circle center to the cursor position.
    //UF_DISP_display_ogp_line(motion_cb_data->view_tag, my_data->abs_ctr, screen_pos);
    str_text[0] = "1";
    str_text[1] = "2";
    str_text[2] = "3";

    UF_DISP_display_temporary_line
        (motion_cb_data->view_tag, UF_DISP_USE_VIEW_TAG, my_data->abs_ctr, screen_pos, &attrib);
    
    UF_DISP_display_temporary_line
    (motion_cb_data->view_tag, UF_DISP_USE_VIEW_TAG, coord_line, screen_pos, &attrib);

    for (int i = 0; i < count_note; ++i)
    {
        UF_DISP_display_temporary_line
        (motion_cb_data->view_tag, UF_DISP_USE_VIEW_TAG, coord_notes, screen_pos, &attrib);

        UF_DISP_display_temporary_text
        (motion_cb_data->view_tag, UF_DISP_USE_VIEW_TAG, const_cast<char*>(str_text[i].c_str()),
            coord_symbol, UF_DISP_BOTTOMLEFT, &attrib, count_note, 1);
        coord_notes[1] = coord_notes[1] - 10;
        screen_pos[1] = screen_pos[1] - 10;
        coord_symbol[1] = coord_symbol[1] - 10;
    }
   
   

}
#define UF_CALL(X) (report( __FILE__, __LINE__, #X, (X)))
static int report(char* file, int line, char* call, int irc)
{
    if (irc)
    {
        char    messg[133];
        printf("%s, line %d:  %s\n", file, line, call);
        (UF_get_fail_message(irc, messg)) ?
            printf("    returned a %d\n", irc) :
            printf("    returned error %d:  %s\n", irc, messg);
    }
    return(irc);
}







static UF_registered_fn_p_t function_id;
static void my_open_callback(UF_callback_reason_e_t reason,
    const void* application_data, void* user_data)
{
    const tag_p_t
        part = (tag_p_t)application_data;
    char
        part_name[MAX_FSPEC_SIZE + 1];

    /* Note that this function needs to call UF_initialize()
       This requirement was added in V16.
    */
    if (UF_initialize()) return;
    UF_PART_ask_part_name(*part, part_name);
    uc1601(part_name, TRUE);
    UF_terminate();
}

//------------------------------------------------------------------------------
// Initialize static variables
//------------------------------------------------------------------------------
Session *(callout::theSession) = NULL;
UI *(callout::theUI) = NULL;
//------------------------------------------------------------------------------
// Constructor for NX Styler class
//------------------------------------------------------------------------------
callout::callout()
{
    try
    {
        // Initialize the NX Open C++ API environment
        callout::theSession = NXOpen::Session::GetSession();
        callout::theUI = UI::GetUI();
        theDlxFileName = "C:\\Users\\putilov_aa\\source\\repos\\Callout\\Callout_dlx\\callout.dlx";
        theDialog = callout::theUI->CreateDialog(theDlxFileName);
        // Registration of callback functions
        theDialog->AddApplyHandler(make_callback(this, &callout::apply_cb));
        theDialog->AddOkHandler(make_callback(this, &callout::ok_cb));
        theDialog->AddUpdateHandler(make_callback(this, &callout::update_cb));
        theDialog->AddInitializeHandler(make_callback(this, &callout::initialize_cb));
        theDialog->AddDialogShownHandler(make_callback(this, &callout::dialogShown_cb));
    }
    catch(exception& ex)
    {
        //---- Enter your exception handling code here -----
        throw;
    }
}

//------------------------------------------------------------------------------
// Destructor for NX Styler class
//------------------------------------------------------------------------------
callout::~callout()
{
    if (theDialog != NULL)
    {
        delete theDialog;
        theDialog = NULL;
    }
}
//------------------------------- DIALOG LAUNCHING ---------------------------------
//
//    Before invoking this application one needs to open any part/empty part in NX
//    because of the behavior of the blocks.
//
//    Make sure the dlx file is in one of the following locations:
//        1.) From where NX session is launched
//        2.) $UGII_USER_DIR/application
//        3.) For released applications, using UGII_CUSTOM_DIRECTORY_FILE is highly
//            recommended. This variable is set to a full directory path to a file 
//            containing a list of root directories for all custom applications.
//            e.g., UGII_CUSTOM_DIRECTORY_FILE=$UGII_ROOT_DIR\menus\custom_dirs.dat
//
//    You can create the dialog using one of the following way:
//
//    1. USER EXIT
//
//        1) Create the Shared Library -- Refer "Block UI Styler programmer's guide"
//        2) Invoke the Shared Library through File->Execute->NX Open menu.
//
//------------------------------------------------------------------------------
extern "C" DllExport void  ufusr(char *param, int *retcod, int param_len)
{
    callout *thecallout = NULL;
    try
    {
        thecallout = new callout();
        // The following method shows the dialog immediately
        thecallout->Show();
    }
    catch(exception& ex)
    {
        //---- Enter your exception handling code here -----
        callout::theUI->NXMessageBox()->Show("Block Styler", NXOpen::NXMessageBox::DialogTypeError, ex.what());
    }
    if(thecallout != NULL)
    {
        delete thecallout;
        thecallout = NULL;
    }
}

//------------------------------------------------------------------------------
// This method specifies how a shared image is unloaded from memory
// within NX. This method gives you the capability to unload an
// internal NX Open application or user  exit from NX. Specify any
// one of the three constants as a return value to determine the type
// of unload to perform:
//
//
//    Immediately : unload the library as soon as the automation program has completed
//    Explicitly  : unload the library from the "Unload Shared Image" dialog
//    AtTermination : unload the library when the NX session terminates
//
//
// NOTE:  A program which associates NX Open applications with the menubar
// MUST NOT use this option since it will UNLOAD your NX Open application image
// from the menubar.
//------------------------------------------------------------------------------
extern "C" DllExport int ufusr_ask_unload()
{
    //return (int)Session::LibraryUnloadOptionExplicitly;
    return (int)Session::LibraryUnloadOptionImmediately;
    //return (int)Session::LibraryUnloadOptionAtTermination;
}

//------------------------------------------------------------------------------
// Following method cleanup any housekeeping chores that may be needed.
// This method is automatically called by NX.
//------------------------------------------------------------------------------
extern "C" DllExport void ufusr_cleanup(void)
{
    try
    {
        //---- Enter your callback code here -----
    }
    catch(exception& ex)
    {
        //---- Enter your exception handling code here -----
        callout::theUI->NXMessageBox()->Show("Block Styler", NXOpen::NXMessageBox::DialogTypeError, ex.what());
    }
}

int callout::Show()
{
    try
    {
        theDialog->Show();
    }
    catch(exception& ex)
    {
        //---- Enter your exception handling code here -----
        callout::theUI->NXMessageBox()->Show("Block Styler", NXOpen::NXMessageBox::DialogTypeError, ex.what());
    }
    return 0;
}

//------------------------------------------------------------------------------
//---------------------Block UI Styler Callback Functions--------------------------
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//Callback Name: initialize_cb
//------------------------------------------------------------------------------
void callout::initialize_cb()
{
    try
    {
        group0 = dynamic_cast<NXOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group0"));
        point0 = dynamic_cast<NXOpen::BlockStyler::SpecifyPoint*>(theDialog->TopBlock()->FindBlock("point0"));
    }
    catch(exception& ex)
    {
        //---- Enter your exception handling code here -----
        callout::theUI->NXMessageBox()->Show("Block Styler", NXOpen::NXMessageBox::DialogTypeError, ex.what());
    }
}

//------------------------------------------------------------------------------
//Callback Name: dialogShown_cb
//This callback is executed just before the dialog launch. Thus any value set 
//here will take precedence and dialog will be launched showing that value. 
//------------------------------------------------------------------------------
void callout::dialogShown_cb()
{
    try
    {
        //---- Enter your callback code here -----
    }
    catch(exception& ex)
    {
        //---- Enter your exception handling code here -----
        callout::theUI->NXMessageBox()->Show("Block Styler", NXOpen::NXMessageBox::DialogTypeError, ex.what());
    }
}

//------------------------------------------------------------------------------
//Callback Name: apply_cb
//------------------------------------------------------------------------------
int callout::apply_cb()
{
    int errorCode = 0;
    try
    {
        //---- Enter your callback code here -----
    }
    catch(exception& ex)
    {
        //---- Enter your exception handling code here -----
        errorCode = 1;
        callout::theUI->NXMessageBox()->Show("Block Styler", NXOpen::NXMessageBox::DialogTypeError, ex.what());
    }
    return errorCode;
}

//------------------------------------------------------------------------------
//Callback Name: update_cb
//------------------------------------------------------------------------------
int callout::update_cb(NXOpen::BlockStyler::UIBlock* block)
{
    try
    {
        if (block == point0)
        {
       
            //---------Enter your code here-----------
            //create_custom_symbol();
           /* const char* ch = "t";
            int num_lines_text = 1;
            char* text_string[1] = { const_cast<char*>(ch) };
            double origin_3d[3] = { point0->Point().X, point0->Point().Y, 0.0 };
            int orientationt = 0;
            tag_t note_tag;
            UF_DRF_create_note (num_lines_text, text_string, origin_3d, orientationt, &note_tag );

            UF_DRF_create_gdt_symbol_with_multiple_leaders
            (
                const int num_lines_text,
                const char text_string[][MAX_LINE_LENGTH + 1],
                const double gdt_symbol_origin[3],
                const UF_DRF_gdt_leader_t * лидер,
                const UF_DRF_frame_corner_tframe_corner,
                tag_t* const gdt_symbol_tag
            )*/
            UF_UI_open_listing_window();
            //do_ugopen_api();

            int                 default_plane;
            int                 plane_resp, ctr_resp, pos_resp;
            tag_t               plane_tag, saved_wcs, mtx_id, csys_id,
                                arc_id, view_tag;
            double              plane_matrix[9], plane_origin[3],
                                x_vec[3], y_vec[3], arc_edge_pos[3];
            double              root_origin[3] = { 0.0, 0.0, 0.0 };
            double              work_origin[3];
            UF_CURVE_arc_t      arc;
            my_motion_data_t    my_data;

            my_data.arc = &arc;
            arc.start_angle = 0.0;
            arc.end_angle = TWOPI;    // Create a full circle. 
            default_plane = 5;       // Default plane = WCS 
          
            UF_CSYS_ask_wcs(&saved_wcs);
            UF_MTX3_x_vec(plane_matrix, x_vec);
            UF_MTX3_y_vec(plane_matrix, y_vec);

            UF_CSYS_map_point(UF_CSYS_ROOT_COORDS, x_vec, UF_CSYS_WORK_COORDS, x_vec);
            UF_CSYS_map_point(UF_CSYS_ROOT_COORDS, y_vec, UF_CSYS_WORK_COORDS, y_vec);
            UF_CSYS_map_point(UF_CSYS_ROOT_COORDS, root_origin, UF_CSYS_WORK_COORDS, work_origin);

            UF_VEC3_sub(x_vec, work_origin, my_data.x_axis);
            UF_VEC3_sub(y_vec, work_origin, my_data.y_axis);

            UF_MTX3_initialize(my_data.x_axis, my_data.y_axis, my_data.matrix);
            UF_CSYS_create_matrix(my_data.matrix, &arc.matrix_tag);

           
           // UF_UI_specify_screen_position(
           //     const_cast<char*>("Specify arc center"),
           //     NULL, NULL, my_data.abs_ctr, &view_tag, &ctr_resp);
            my_data.abs_ctr[0] = point0->Point().X;
            my_data.abs_ctr[1] = point0->Point().Y;
            my_data.abs_ctr[2] = point0->Point().Z;

            UF_MTX3_vec_multiply(my_data.abs_ctr, my_data.matrix, arc.arc_center);
            UF_UI_specify_screen_position(
                const_cast<char*>("Indicate arc radius"),
                (UF_UI_motion_fn_t)motion_cb, (void*)&my_data, arc_edge_pos, &view_tag, &pos_resp);
        
            create_callout(arc_edge_pos, &my_data, point0);

            UF_DISP_refresh();

            //create_callout(point0->Point().X, point0->Point().Y);
            // 
            // if (UF_initialize()) return;
            //if (!UF_add_callback_function(UF_open_part_reason, my_open_callback, NULL, &function_id)) 
            //    uc1601(const_cast<char*>("Added callback"), TRUE);
            //UF_terminate();
        
            //SpecifyScreenPosition
           // UF_UI_specify_screen_position(...)
           //     UF_UI_motion_fn_t motion_cb(...)

            //UF_UI_selection_p_t select_;
            //tag_t view;
            //double abs_cursor_pos[3];

//            UF_UI_ask_sel_cursor_pos (select_, &view, abs_cursor_pos);

            //test();
            
            

            theDialog->PerformApply();
            
            
        }
    }
    catch(exception& ex)
    {
        //---- Enter your exception handling code here -----
        callout::theUI->NXMessageBox()->Show("Block Styler", NXOpen::NXMessageBox::DialogTypeError, ex.what());
    }
    return 0;
}

//------------------------------------------------------------------------------
//Callback Name: ok_cb
//------------------------------------------------------------------------------
int callout::ok_cb()
{
    int errorCode = 0;
    try
    {
        errorCode = apply_cb();
    }
    catch(exception& ex)
    {
        //---- Enter your exception handling code here -----
        errorCode = 1;
        callout::theUI->NXMessageBox()->Show("Block Styler", NXOpen::NXMessageBox::DialogTypeError, ex.what());
    }
    return errorCode;
}

//------------------------------------------------------------------------------
//Function Name: GetBlockProperties
//Description: Returns the propertylist of the specified BlockID
//------------------------------------------------------------------------------
PropertyList* callout::GetBlockProperties(const char *blockID)
{
    return theDialog->GetBlockProperties(blockID);
}



/*
static void do_ugopen_api(void)
{
    char cue[] = "Select Objects";
    char title[] = "User Title";
    int response, count, i;
    tag_p_t objects;
    user_data_t data;
    // initialize maximum number of entities to be selected 
    data.max_count = 25;
    // call class selection dialog 
    if ((UF_UI_select_with_class_dialog(cue, title, UF_UI_SEL_SCOPE_WORK_PART, init_proc, &data, &response, &count, &objects)) == 0)
    {
        printf("object count = %d\n", count);
        if (response == UF_UI_OK && count > 0)
        {
            for (i = 0; i < count; i++)
            {
                printf("object tag = %d\n", objects[i]);
                UF_DISP_set_highlight(objects[i], 0);
            }
            UF_free(objects);
        }
    }
}*/


static int init_proc( UF_UI_selection_p_t select, void* user_data)
{
   // tag_t view;
   // double abs_cursor_pos[3];
    /* specify filter procedure for this dialog */
    if (UF_UI_ask_sel_cursor_pos(select, &view, abs_cursor_pos) == 0)
    {
        return (UF_UI_SEL_SUCCESS);
    }
    else
    {
        return (UF_UI_SEL_FAILURE);
    }
}

/*static int init_proc(UF_UI_selection_p_t select, void* user_data)
{
    // enable faces to be selectable 
    UF_UI_mask_t mask_triples[] = {
        UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE };
    if (UF_UI_set_sel_mask(select, UF_UI_SEL_MASK_ENABLE_SPECIFIC, 1, mask_triples))
    {
        return (UF_UI_SEL_FAILURE);
    }
    // specify selection callback 
    if ((UF_UI_set_sel_procs(select, NULL, sel_cb, user_data)) == 0)
    {
        return (UF_UI_SEL_SUCCESS);
    }
    else
    {
        return (UF_UI_SEL_FAILURE);
    }
}*/
/* user selection callback
   if a face was deselected, deselect all adjacent faces.
   when more than max_count objects have been selected,
   terminate the dialog */
static int sel_cb(
    int num_selected,
    tag_p_t objects,
    int num_deselected,
    tag_p_t deselected_objects,
    void* user_data,
    UF_UI_selection_p_t select)
{
    int type, subtype, i, j, count;
    tag_t face;
    logical in_list, unhilite = true;
    uf_list_p_t adj_faces;
    user_data_p_t data = (user_data_p_t)user_data;
    /* if objects were deselected */
    if (num_deselected > 0)
    {
        /* loop over deselected objects */
        for (i = 0; i < num_deselected; i++)
        {
            /* if object is a face */
            UF_OBJ_ask_type_and_subtype
            (deselected_objects[i], &type, &subtype);
            if (type == UF_solid_type &&
                subtype == UF_solid_face_subtype)
            {
                /* get all adjacent faces */
                UF_MODL_ask_adjac_faces(deselected_objects[i],
                    &adj_faces);
                UF_MODL_ask_list_count(adj_faces, &count);
                /* remove the adjacent faces from the selection
                   list and turn off their highlighting */
                for (j = 0; j < count; j++)
                {
                    UF_MODL_ask_list_item(adj_faces, j, &face);
                    if (face != NULL_TAG)
                    {
                        UF_UI_is_object_in_sel_list
                        (select, face, &in_list);
                        if (in_list)
                        {
                            UF_UI_remove_from_sel_list
                            (select, 1, &face, unhilite);
                        }
                    }
                }
                UF_MODL_delete_list(&adj_faces);
            }
        }
    }
    /* else if objects selected, check max count */
    else if (num_selected > 0)
    {
        /* ask the number of objects currently selected */
        if ((UF_UI_ask_sel_list_count(select, &count)) == 0)
        {
            /*exit dialog if maximum number of objects have been
              selected */
            if (count > data->max_count)
            {
                return (UF_UI_CB_EXIT_DIALOG);
            }
        }
    }
    return (UF_UI_CB_CONTINUE_DIALOG);
}


/*
static void do_ugopen_api(void)
{
    int               default_plane;
    int               plane_resp, ctr_resp, pos_resp;
    tag_t             plane_tag, saved_wcs, mtx_id, csys_id,
        arc_id, view_tag;
    double            plane_matrix, plane_origin,
        x_vec, y_vec, arc_edge_pos;
    double         root_origin = 0.0;
    double         work_origin;
    UF_CURVE_arc_t    arc;
    my_motion_data_t my_data;

    my_data.arc = &arc;
    arc.start_angle = 0.0;
    arc.end_angle = TWOPI;    // Create a full circle. 
    default_plane = 5;       // Default plane = WCS 
    do
    {
        // Specify the plane on which the circle is to be created.
           
        UF_UI_specify_plane(
            const_cast<char*>("Specify plane for circle creation"),
            &default_plane,
            1,
            &plane_resp,
            &plane_matrix,
            &plane_origin,
            &plane_tag);
        if (plane_resp == 3)
        {
            // Save the current WCS for future restoration.
             
            UF_CSYS_ask_wcs(&saved_wcs);
            // Move the WCS to the specified plane. This is
             // necessary because the position passed to the motion
             // callback, and the position returned by
             // UF_UI_specify_screen_position, is the screen
             // position projected onto the WCS XY plane.
             
            if (default_plane != 5)
            {
                UF_CSYS_create_matrix(&plane_matrix, &mtx_id);
                UF_CSYS_create_csys(&plane_origin, mtx_id,
                    &csys_id);
                UF_CSYS_set_wcs(csys_id);
            }
            // Obtain unit vectors and the arc matrix relative to
             // the Work Part coordinate system.
             
            UF_MTX3_x_vec(&plane_matrix, &x_vec);
            UF_MTX3_y_vec(&plane_matrix, &y_vec);
            UF_CSYS_map_point(UF_CSYS_ROOT_COORDS, &x_vec,
                UF_CSYS_WORK_COORDS, &x_vec);
            UF_CSYS_map_point(UF_CSYS_ROOT_COORDS, &y_vec,
                UF_CSYS_WORK_COORDS, &y_vec);
            UF_CSYS_map_point(UF_CSYS_ROOT_COORDS, &root_origin,
                UF_CSYS_WORK_COORDS, &work_origin);
            UF_VEC3_sub(&x_vec, &work_origin, &my_data.x_axis);
            UF_VEC3_sub(&y_vec, &work_origin, &my_data.y_axis);
            UF_MTX3_initialize(&my_data.x_axis, &my_data.y_axis,
                &my_data.matrix);
            UF_CSYS_create_matrix(&my_data.matrix,
                &arc.matrix_tag);
            do
            {
                UF_UI_specify_screen_position(
                    const_cast<char*>("Specify arc center"),
                    NULL,
                    NULL,
                    &my_data.abs_ctr,
                    &view_tag,
                    &ctr_resp);
                if (ctr_resp == UF_UI_PICK_RESPONSE)
                {
                    // Map the arc center to the arc csys.
                       
                    UF_MTX3_vec_multiply(&my_data.abs_ctr, &my_data.matrix, arc.arc_center);
                    UF_UI_specify_screen_position(
                        const_cast<char*>("Indicate arc radius"),
                        (UF_UI_motion_fn_t)motion_cb, (void*)&my_data, &arc_edge_pos, &view_tag, &pos_resp);
                    // If a position was obtained, create the circle.
                       
                    if (pos_resp == UF_UI_PICK_RESPONSE)
                    {
                        UF_VEC3_distance(&my_data.abs_ctr, &arc_edge_pos, &arc.radius);
                        UF_CURVE_create_arc(&arc, &arc_id);
                    }
                }
            } while ((ctr_resp == UF_UI_PICK_RESPONSE) &&
                (pos_resp != UF_UI_CANCEL));
            // Restore the WCS if it was altered above.
             
            if (default_plane != 5)
            {
                UF_CSYS_set_wcs(saved_wcs);
                UF_OBJ_delete_object(csys_id);
            }
        }
    } while (ctr_resp == UF_UI_BACK && plane_resp == 3);
}*/

void do_ugopen_api(void)
{
    int                 default_plane;
    int                 plane_resp, ctr_resp, pos_resp;
    tag_t               plane_tag, saved_wcs, mtx_id, csys_id,
                        arc_id, view_tag;
    double              plane_matrix[9], plane_origin[3],
                        x_vec[3], y_vec[3], arc_edge_pos[3];
    double              root_origin[3] = { 0.0, 0.0, 0.0 };
    double              work_origin[3];
    UF_CURVE_arc_t      arc;
    my_motion_data_t    my_data;

    my_data.arc = &arc;
    arc.start_angle = 0.0;
    arc.end_angle = TWOPI;    /* Create a full circle. */
    default_plane = 5;       /* Default plane = WCS */
    //do
    //{
        // Specify the plane on which the circle is to be created.
         
        //UF_UI_specify_plane(
        //    const_cast<char*>("Specify plane for circle creation"), &default_plane, 1, &plane_resp, plane_matrix, plane_origin, &plane_tag);

       // if (plane_resp == 3)
       // {
            // Save the current WCS for future restoration.
             
            UF_CSYS_ask_wcs(&saved_wcs);
            // Move the WCS to the specified plane. This is
             // necessary because the position passed to the motion
             // callback, and the position returned by
             // UF_UI_specify_screen_position, is the screen
             // position projected onto the WCS XY plane.
             
         //   if (default_plane != 5)
         //   {
         //       UF_CSYS_create_matrix(plane_matrix, &mtx_id);
         //       UF_CSYS_create_csys(plane_origin, mtx_id, &csys_id);
         //       UF_CSYS_set_wcs(csys_id);
         //   }
            // Obtain unit vectors and the arc matrix relative to the Work Part coordinate system.
             
            UF_MTX3_x_vec(plane_matrix, x_vec);
            UF_MTX3_y_vec(plane_matrix, y_vec);

            UF_CSYS_map_point(UF_CSYS_ROOT_COORDS, x_vec, UF_CSYS_WORK_COORDS, x_vec);
            UF_CSYS_map_point(UF_CSYS_ROOT_COORDS, y_vec, UF_CSYS_WORK_COORDS, y_vec);
            UF_CSYS_map_point(UF_CSYS_ROOT_COORDS, root_origin, UF_CSYS_WORK_COORDS, work_origin);

            UF_VEC3_sub(x_vec, work_origin, my_data.x_axis);
            UF_VEC3_sub(y_vec, work_origin, my_data.y_axis);

            UF_MTX3_initialize(my_data.x_axis, my_data.y_axis, my_data.matrix);
            UF_CSYS_create_matrix(my_data.matrix, &arc.matrix_tag);

            //do
            //{
                UF_UI_specify_screen_position(
                    const_cast<char*>("Specify arc center"),
                    NULL, NULL, my_data.abs_ctr, &view_tag, &ctr_resp);
                if (ctr_resp == UF_UI_PICK_RESPONSE)
                {
                    // Map the arc center to the arc csys.
                     
                    
                    
                    UF_MTX3_vec_multiply(my_data.abs_ctr, my_data.matrix, arc.arc_center);
                    UF_UI_specify_screen_position(
                        const_cast<char*>("Indicate arc radius"),
                        (UF_UI_motion_fn_t)motion_cb, (void*)&my_data, arc_edge_pos, &view_tag, &pos_resp);
                    // If a position was obtained, create the circle.
                     
                   // if (pos_resp == UF_UI_PICK_RESPONSE)
                   // {
                    //    UF_VEC3_distance(my_data.abs_ctr, arc_edge_pos, &arc.radius);
                    //    UF_CURVE_create_arc(&arc, &arc_id);
                    //}
                }
           // } while ((ctr_resp == UF_UI_PICK_RESPONSE) && (pos_resp != UF_UI_CANCEL));
            // Restore the WCS if it was altered above. 
 //           if (default_plane != 5)
 //           {
 //               UF_CSYS_set_wcs(saved_wcs);
//                UF_OBJ_delete_object(csys_id);
 //           }
       // }
    //} while (ctr_resp == UF_UI_BACK);&& plane_resp == 3);
    //create_callout(arc_edge_pos, &my_data);
    //create_callout(arc_edge_pos[0], arc_edge_pos[1], &my_data);
    UF_DISP_refresh();
}
static void do_ugopen_api1(void) //set color
{
    int     count, clr_num, name_len;
    char* clr_name;
    char* new_name;
    double  clr_values[3];
    UF_DISP_ask_color_count(&count);
    for (clr_num = 1; clr_num < count; clr_num++)
    {
        // Request "hsv" color values 
        UF_DISP_ask_color(clr_num, UF_DISP_hsv_model, &clr_name, clr_values);
        // Reduce the saturation by 10% 
        clr_values[1] *= 0.9;
        // Append "_dil" (i.e. diluted) to the color name 
        name_len = strlen(clr_name);
        new_name = (char*)malloc(name_len + 5);
        strcpy(new_name, clr_name);
        if (name_len < (UF_DISP_MAX_NAME_SIZE - 4))
        {
            strcat(new_name, "_dil");
        }
        UF_DISP_set_color(clr_num, UF_DISP_hsv_model, new_name, clr_values);
        free(new_name);
        UF_free(clr_name);
    }
    // Set background color to blue using "rgb" model. 
    // Color name is ignored for background.           
    clr_values[0] = 0.0;          // red   
    clr_values[1] = 0.0;          // green 
    clr_values[2] = 1.0;          // blue  
    UF_DISP_set_color(UF_DISP_BACKGROUND_COLOR, UF_DISP_rgb_model, NULL, clr_values);

    UF_DISP_load_color_table();
    // Determine the closest color to blue.  Note that 
    // the background color is not considered.         
    clr_values[0] = 240.0;
    clr_values[1] = 0.0;
    clr_values[2] = 0.0;
    UF_DISP_ask_closest_color(
        UF_DISP_hsv_model,
        clr_values,
        UF_DISP_CCM_EUCLIDEAN_DISTANCE,
        &clr_num);
    printf("The closest color to blue is #: %d", clr_num);
}

static void create_callout(double coordX, double coordY, my_motion_data_t* my_data)
{
    //my_motion_data_t* my_data;


    int status = 0;
    int lines_of_text = 1;
    static double  origin[3] = { coordX, coordY, 0.0};
    static double base_pt[3] = { my_data->abs_ctr[0], my_data->abs_ctr[1], 0.0 };
    char text[1][MAX_LINE_BUFSIZE] = { " F " };
    char stat_msg[133];
    tag_t base_pt_tag, text_aid_tag;
    UF_DRF_object_t object;
    status = UF_initialize();
    if (!status)
    {
        // initialize the object structures 
        UF_DRF_init_object_structure(&object);
        // create a base point 
        status = UF_CURVE_create_point(base_pt, &base_pt_tag);
    }
    if (!status)
    {
        // create a label 
        strcpy(text[0], " А ");
        /*strcpy(text[1], "It points to the screen position at (20, 5).");
        strcpy(text[2], "The next line is BLANK.");
        strcpy(text[3], "");
        strcpy(text[4], "The next line is very long....");
        strcpy(text[5], "ABCDEFGHIJKLMNOPQRSTUVWXYZYXWV");
        strcpy(text[6], "This is the LAST line.");*/
        object.object_tag = base_pt_tag;
        object.object_view_tag = NULL_TAG;
        object.object_assoc_type = UF_DRF_end_point;
        object.object_assoc_modifier = UF_DRF_last_end_point;

        status = UF_DRF_create_label(lines_of_text, text, origin,
            UF_DRF_leader_attach_screen, &object, base_pt,
            &text_aid_tag);

       

    }
    printf("UF_DRF_create_label ");
    if (status)
    {
        UF_get_fail_message(status, stat_msg);
        printf("has ERROR %d: %s.\n", status, stat_msg);
    }
    else
        printf("is successful.\n");
    UF_terminate();
}


void create_callout(double coord[], my_motion_data_t* my_data, NXOpen::BlockStyler::SpecifyPoint* point0)
{
    //my_motion_data_t* my_data;
    //NXOpen::BlockStyler::SpecifyPoint* point = callout::GetObject();
    

    test1();

    int status = 0;
    int lines_of_text = 1;
    double  origin[3] = { coord[0], coord[1], coord[2]};
    double base_pt[3] = { my_data->abs_ctr[0], my_data->abs_ctr[1], my_data->abs_ctr[2] };
    char text[1][MAX_LINE_BUFSIZE];
    char stat_msg[133];
    tag_t base_pt_tag, text_aid_tag, t1,t2;
    UF_DRF_object_t object;

    UF_CURVE_line_t line_coords;
    
 
    status = UF_initialize();
    if (!status)
    {
        // initialize the object structures 
        UF_DRF_init_object_structure(&object);
        // create a base point 
        status = UF_CURVE_create_point(base_pt, &base_pt_tag);
    }
    if (!status)
    {
        // create a label 
        strcpy(text[0], " XO ");
        /*strcpy(text[1], "It points to the screen position at (20, 5).");
        strcpy(text[2], "The next line is BLANK.");
        strcpy(text[3], "");
        strcpy(text[4], "The next line is very long....");
        strcpy(text[5], "ABCDEFGHIJKLMNOPQRSTUVWXYZYXWV");
        strcpy(text[6], "This is the LAST line.");*/
        object.object_tag = base_pt_tag;
        object.object_view_tag = NULL_TAG;
        object.object_assoc_type = UF_DRF_end_point;
        object.object_assoc_modifier = UF_DRF_first_end_point;

        //UF_DRF_text_above_leader_t leader = UF_DRF_LEADER_TOP_TEXT_MAX_UNDERLINE;
        //UF_DRF_set_text_above_leader ( leader );
        
        status = UF_DRF_create_label(lines_of_text, text, origin,
            UF_DRF_leader_attach_screen, &object, base_pt, &text_aid_tag);

        //status = UF_DRF_create_id_symbol(UF_DRF_sym_underline,
        //    text[0], text[0], origin, UF_DRF_with_leader,
        //    UF_DRF_leader_attach_screen, &object, base_pt,
        //    &text_aid_tag);

        //status = UF_DRF_create_gdt_symbol(lines_of_text, text, origin,
         //   UF_DRF_leader_type_line, UF_DRF_leader_attach_screen,
         //   &object, base_pt, UF_DRF_frame_none, &text_aid_tag);

        /*double              gdt_symbol_origin[3];
        UF_DRF_gdt_leader_t* leader;
        tag_t              gdt_symbol_tag;
        UF_DRF_create_gdt_symbol_with_multiple_leaders (1, text, 
            gdt_symbol_origin, leader, UF_DRF_frame_none, &gdt_symbol_tag );*/
        
        //UF_DRF_object_assoc_data_p_t associativity_data;
        //UF_DRF_init_associativity_data (associativity_data);
       


/* NXOpen::Session* theSession = NXOpen::Session::GetSession();
NXOpen::Part* workPart(theSession->Parts()->Work());
NXOpen::Part* displayPart(theSession->Parts()->Display());
NXOpen::Session::UndoMarkId markId1;
markId1 = theSession->SetUndoMark(NXOpen::Session::MarkVisibilityVisible, "Edit Object Origin");

NXOpen::Annotations::Label* label1(dynamic_cast<NXOpen::Annotations::Label*>(theSession->);
label1->SetLeaderOrientation(NXOpen::Annotations::LeaderOrientationFromRight);

NXOpen::Point3d origin1(78.3196248423707, 172.109252837327, 0.0);
label1->SetAnnotationOrigin(origin1);

int nErrs1;
nErrs1 = theSession->UpdateManager()->DoUpdate(markId1);

NXOpen::Session::UndoMarkId markId2;
markId2 = theSession->SetUndoMark(NXOpen::Session::MarkVisibilityVisible, "Edit Object Origin");

label1->SetLeaderOrientation(NXOpen::Annotations::LeaderOrientationFromLeft);

NXOpen::Point3d origin2(119.779776166456, 168.654240226986, 0.0);
label1->SetAnnotationOrigin(origin2);

int nErrs2;
nErrs2 = theSession->UpdateManager()->DoUpdate(markId2);*/









        
       
/*
tag_t call;
UF_DRF_ask_callout_of_annotation (text_aid_tag, &call );
    std::string s0 = std::to_string(call);
    //UF_UI_write_listing_window((s0 + "\n").c_str());

tag_t exp_id[2];
UF_DRF_ask_custom_symbol_attach_locations(text_aid_tag, exp_id);//место крепления выноски
    std::string s1 = std::to_string(exp_id[0]);
    std::string s2 = std::to_string(exp_id[1]);
    //UF_UI_write_listing_window((s1 + " " + s2 + "\n").c_str());

UF_DRF_associative_origin_p_t origin_data1;
double origin1[3];
UF_DRF_ask_associative_origin(text_aid_tag, &origin_data1, origin1);
    std::string s3 = std::to_string(origin1[1]);
    UF_UI_write_listing_window((s3 + "\n").c_str());

UF_DRF_set_associative_origin(text_aid_tag, origin_data1, origin1);


UF_DRF_leader_data_p_t Leader_data;
UF_DRF_ask_custom_symbol_leader(text_aid_tag, &Leader_data);
    std::string s4 = std::to_string(Leader_data->num_leaders);
    UF_UI_write_listing_window((s4 + "\n").c_str());
UF_DRF_free_leader_data(&Leader_data);*/
//------------------------------------------------
/*int num_associativities;
UF_DRF_object_assoc_data_p_t associativity_data;
UF_DRF_ask_associativity_data (text_aid_tag, &num_associativities, &associativity_data);
std::string s3 = std::to_string(num_associativities);
UF_UI_write_listing_window((s3 + "\n").c_str());


UF_DRF_set_associativity_data( text_aid_tag, num_associativities, associativity_data );
UF_free(&associativity_data);*/
//-----------------------------------------------

UF_DRF_associative_origin_p_t origin_data;
double start[3];
//UF_DRF_ask_associative_origin (text_aid_tag, &origin_data, start );

double up_left[3];
double lengtch;
double height;
UF_DRF_ask_annotation_text_box (text_aid_tag, up_left, &lengtch, &height);


tag_t t[1];
t[0] = text_aid_tag;
int mask[4];
mask[0] = 1;
mask[1] = 0;
mask[2] = 0;
mask[3] = 0;
int Cycle_flag = 0;
int ann_data[10];

int ann_data_type;
int ann_data_form;
int num_segments;
double ann_origin[2];
double radius_angle;

UF_DRF_ask_ann_data
(t, mask, &Cycle_flag, ann_data,
    &ann_data_type, &ann_data_form, &num_segments, ann_origin, &radius_angle);


tag_t np1[1];
np1[0] = text_aid_tag;
int ir2;
uf5554 ( np1, &ir2 );



        //******************** Ручное создание линий и символов ***************************** 
        /*char                          error_message[133] = "";
        int                           ifail = 0;
        int                           type, subtype;
        tag_t                         label_tag = NULL_TAG;
        double                        label_origin[3];
        UF_DRF_label_info_t* label_info;

        ifail = UF_DRF_ask_label_info(text_aid_tag, &label_origin[0], &label_info);
            std::string s = std::to_string(label_origin[0]);
            UF_UI_write_listing_window((s+"\n").c_str());

        
        double symbol_length = label_info->text_info->length;
        double symbol_height = label_info->text_info->height;
        double distance = label_info->text_info->distance;
        double cor[3];
        //UF_DRF_ask_origin(origin_data->horiz_annotation, cor);

        line_coords.start_point[0] = label_info->leader_info->leader_pnts->segment_pnts[0];
        line_coords.start_point[1] = label_info->leader_info->leader_pnts->segment_pnts[1];
        line_coords.start_point[2] = label_info->leader_info->leader_pnts->segment_pnts[2];

        line_coords.end_point[0] = label_info->leader_info->leader_pnts->segment_pnts[0];
        line_coords.end_point[1] = label_info->leader_info->leader_pnts->segment_pnts[1] - (10 * (count_note - 1));// symbol_height;
        line_coords.end_point[2] = label_info->leader_info->leader_pnts->segment_pnts[2];

        UF_CURVE_create_line(&line_coords, &t1);//линия от 1 полки до n

        int num_lines_text = 1;
        char* ch = const_cast<char*>("text");
        char* text_string[1] = { ch };
        double origin_3d[3] = { label_info->text_info->origin[0]-8, label_info->text_info->origin[1]+2, 0.0 };
        int orientationt = 0;
        tag_t note_tag;
        //UF_DRF_create_note(num_lines_text, text_string, origin_3d, orientationt, &note_tag);//текст

        line_coords.start_point[0] = label_info->leader_info->leader_pnts->segment_pnts[0];
        line_coords.start_point[1] = label_info->leader_info->leader_pnts->segment_pnts[1];
        line_coords.start_point[2] = label_info->leader_info->leader_pnts->segment_pnts[2];

        line_coords.end_point[0] = label_info->leader_info->leader_pnts->segment_pnts[0] - 10;
        line_coords.end_point[1] = label_info->leader_info->leader_pnts->segment_pnts[1];// symbol_height;
        line_coords.end_point[2] = label_info->leader_info->leader_pnts->segment_pnts[2];

        tag_t tag_lines[count_note];
        tag_t tag_notes[count_note];

        //Создание линий и символов
        for (int i = 0; i < count_note; ++i)
        {
            UF_CURVE_create_line(&line_coords, &tag_lines[i]);//линия
            UF_DRF_create_note(num_lines_text, text_string, origin_3d, orientationt, &tag_notes[i]);//текст
            line_coords.start_point[1] = line_coords.start_point[1] - 10;
            line_coords.end_point[1] = line_coords.end_point[1] - 10;
            origin_3d[1] -= 10;
        }

tag_t exp_id1;
UF_DRF_ask_controlling_exp (text_aid_tag, &exp_id1 );
    std::string s12 = std::to_string(exp_id1);
    UF_UI_write_listing_window(("control exp - " + s12 + "\n").c_str());

        //Добавление линий и символов к стрелке
        const int number_of_objects = count_note + count_note + 1;
        tag_t arr_tag[number_of_objects];
        arr_tag[0] = t1;
        int count_ind = 0;
        for (int i = 0; i < count_note; ++i)
        {
            arr_tag[count_ind + 1] = tag_lines[i];
            arr_tag[count_ind + 2] = tag_notes[i];
            count_ind += 2;
            //std::string s4 = std::to_string(arr_tag[i]);
            //UF_UI_write_listing_window((s4 + "\n").c_str());
            //UF_DRF_add_controlling_exp (tag_lines[i], exp_id1);
        }
        UF_DRF_add_assortpart_to_ann(text_aid_tag, number_of_objects, arr_tag);

        
        //UF_DRF_add_assortpart_to_ann (text_aid_tag, 1, arr_tag);
        //if (!ifail)
        UF_DRF_free_label(&label_info);
        //**************************************************************************************************/


      /*
        int status1 = 0;
        static double  origin1[3] = { 100.0, 100.0, 0.0 };
        static double base_pt1[3] = { 110.0, 110.0, 0.0 };
        char text1[10][132 + 1];
        char stat_msg1[133];
        tag_t base_pt_tag1, text_aid_tag1;
        UF_DRF_object_t object1;
        status1 = UF_initialize();
        if (!status1)
        {
            // initialize the object structures 
            UF_DRF_init_object_structure(&object1);
            // create a base point 
            status = UF_CURVE_create_point(base_pt1, &base_pt_tag1);
        }
        if (!status1)
        {
            // create an id symbol 
            strcpy(text1[0], "ID");
            strcpy(text1[1], "LO");
            object1.object_tag = base_pt_tag1;
            object1.object_view_tag = NULL_TAG;
            object1.object_assoc_type = UF_DRF_intersection;
            object1.object_assoc_modifier = UF_DRF_first_end_point;
            //object1.assoc_dwg_pos

            status1 = UF_DRF_create_id_symbol(UF_DRF_sym_underline,
                text1[0], text1[1], origin1, UF_DRF_with_leader,
                UF_DRF_leader_attach_screen, &object1, base_pt1,
                &text_aid_tag1);
        }

    /*   //UF_DRF_create_gdt_symbol_with_multiple_leaders
        //(1, text, gdt_symbol_origin, lid, UF_DRF_frame_none, &gdt_symbol_tag );

        int status3 = 0;
        int lines_of_text3 = 1;
        static double  origin3[3] = { 24.0, 7.0, 0.0 };
        static double base_pt3[3] = { 20.0, 5.0, 0.0 };
        char text3[1][MAX_LINE_BUFSIZE];
        char stat_msg3[133];
        tag_t text_aid_tag3;
        tag_t base_pt_tag3;
        UF_DRF_object_t object3;
        status3 = UF_initialize();
        if (!status3)
        {
            // initialize the object structures 
            UF_DRF_init_object_structure(&object3);
            // create a base point 
            status = UF_CURVE_create_point(base_pt3, &base_pt_tag3);
        }
        if (!status)
        {
            // create a geometric dimensioning and tolerancing symbol 
            strcpy(text3[0], "GEOMETRIC DIMENSIONING     <+>");
            /*strcpy(text3[1], "AND TOLERANCING SYMBOLS    <+>");
            strcpy(text3[2], "<M><+><(><S><)><+><O>< +> ");
            strcpy(text3[3], "<&1><&2><&3><&4><&5&gt; <&6> < +>");
            strcpy(text3[4], "<&7><+><&8><+><&9>");
            strcpy(text3[5], "<&10><&11><&12><&13><&amp; 14 > ");
            strcpy(text3[6], "It points to the screen position at(20, 5).");
            strcpy(text3[7], "This is the LAST line<+>");*/
     /*      object3.object_tag = NULL_TAG;
            object3.object_view_tag = NULL_TAG;
            object3.object_assoc_type = UF_DRF_end_point;
            object3.object_assoc_modifier = UF_DRF_first_end_point;
            status3 = UF_DRF_create_gdt_symbol(lines_of_text3, text3, origin3,
                UF_DRF_leader_type_line,
                UF_DRF_leader_attach_screen,
                &object3, base_pt3, UF_DRF_frame_none, &text_aid_tag3);
        }*/


    }
    printf("UF_DRF_create_label ");
    if (status)
    {
        UF_get_fail_message(status, stat_msg);
        printf("has ERROR %d: %s.\n", status, stat_msg);
    }
    else
        printf("is successful.\n");
    UF_terminate();
}




void test()
{
    int     volatile status = 0;
    int     num_symbol_fonts = 2;
    logical is_inverted = TRUE;
    logical is_mirrored = TRUE;
    double  symbol_factor = 0.0;
    double  symbol_anchor[3] = { -1.5, 0.0, 0.0 };
    double  symbol_orient[3] = { 1.5, 0.0, 0.0 };
    double  symbol_angle = 30.0;
    double  symbol_length = 2.0;
    double  symbol_height = 1.0;
    double  point1[3];
    tag_t   point1_tag, point2_tag, point3_tag;
    tag_t   line1_tag, line2_tag;
    tag_t   symbol_tag, symbol_font_tags[9];
    tag_t   dimension_tag;
    char    dimtxt[15 + 1] = "DIMENSION TEXT ";
    char    symbol_file_name[SYMBOL_FILE_NAME_SIZE + 1];
    char    symbol_name[SYMBOL_NAME_SIZE + 1] = "SYMBOLx ";
    UF_CURVE_line_t             line_coords;
    UF_DRF_symbol_create_data_t symbol_data;
    UF_DRF_symbol_data_t        symbol_info;
    UF_DRF_object_t             object1, object2;
    UF_DRF_text_t               drf_text;
    status = UF_initialize();
    if (!status)
    {
        /* create some points and lines */
        point1[0] = 2.0;
        point1[1] = 11.0;
        point1[2] = 0.0;
        UF_CURVE_create_point(point1, &point1_tag);
        point1[0] = 3.0;
        point1[1] = 0.5;
        UF_CURVE_create_point(point1, &point2_tag);
        point1[0] = -0.7;
        point1[1] = 2.5;
        UF_CURVE_create_point(point1, &point3_tag);
        line_coords.start_point[0] = -1.0;
        line_coords.start_point[1] = -1.0;
        line_coords.start_point[2] = 0.0;
        line_coords.end_point[0] = 1.0;
        line_coords.end_point[1] = 1.0;
        line_coords.end_point[2] = 0.0;
        UF_CURVE_create_line(&line_coords, &line1_tag);
        line_coords.start_point[0] = 1.0;
        line_coords.start_point[1] = -1.0;
        line_coords.start_point[2] = 0.0;
        line_coords.end_point[0] = -1.0;
        line_coords.end_point[1] = 1.0;
        line_coords.end_point[2] = 0.0;
        UF_CURVE_create_line(&line_coords, &line2_tag);
        UF_DRF_init_symbol_create_data(&symbol_data);
        /* initialize the object structures */
        UF_DRF_init_object_structure(&object1);
        UF_DRF_init_object_structure(&object2);
        /* load drf_text specification */
        drf_text.user_dim_text = dimtxt;
        drf_text.lines_app_text = 0;
        drf_text.appended_text = NULL;
        /* create a parallel dimension */
        point1[0] = 5.0;
        point1[1] = 3.0;
        object1.object_tag = line1_tag;
        object1.object_view_tag = NULL_TAG;
        object1.object_assoc_type = UF_DRF_end_point;
        object1.object_assoc_modifier = UF_DRF_last_end_point;
        object2.object_tag = point1_tag;
        object2.object_view_tag = NULL_TAG;
        object2.object_assoc_type = UF_DRF_end_point;
        object2.object_assoc_modifier = UF_DRF_first_end_point;
        UF_DRF_create_parallel_dim (&object1, &object2, &drf_text, point1, &dimension_tag);
        UF_DRF_set_customer_sbf_file();
        UF_DRF_ask_sbf_file(symbol_file_name);
        /* create symbol font entity */
        symbol_font_tags[0] = line1_tag;
        symbol_font_tags[1] = line2_tag;
        UF_DRF_create_symbol_font
            (symbol_name, symbol_factor, symbol_anchor,
            symbol_orient, num_symbol_fonts, symbol_font_tags);
        /* create standalone symbol */
        strcpy(symbol_data.symbol_name, symbol_name);
        symbol_data.angle = symbol_angle;
        symbol_data.length = symbol_length;
        symbol_data.height = symbol_height;
        symbol_data.anchor_tag = point1_tag;
        UF_DRF_place_symbol (&symbol_data, is_inverted, is_mirrored, &symbol_tag);
        symbol_data.length *= 0.15;
        symbol_data.height *= 0.15;
        symbol_data.anchor_tag = point2_tag;
        UF_DRF_add_symbol_to_object (&symbol_data, dimension_tag);
        symbol_data.anchor_tag = point3_tag;
        UF_DRF_add_symbol_to_object (&symbol_data, dimension_tag);
        UF_DRF_ask_symbols_used (dimension_tag, &num_symbol_fonts, symbol_font_tags);
        status = UF_DRF_ask_symbol_data(symbol_tag, &symbol_info);
        if (status == UF_DRF_NO_ERRORS)
        {
            symbol_font_tags[0] = symbol_info.symbol_font_tag;
            /* absolute anchor point coordinates */
            symbol_anchor[0] = symbol_info.anchor_point[0];
            symbol_anchor[1] = symbol_info.anchor_point[1];
            symbol_anchor[2] = symbol_info.anchor_point[2];
            /* absolute orientation point coordinates */
            symbol_orient[0] = symbol_info.orient_point[0];
            symbol_orient[1] = symbol_info.orient_point[1];
            symbol_orient[2] = symbol_info.orient_point[2];
            /* angle, length, height */
            symbol_angle = symbol_info.angle;
            symbol_length = symbol_info.length;
            symbol_height = symbol_info.height;
        }
    }
    UF_terminate();
}
void test1()
{
    int     volatile status = 0;
    int     num_symbol_fonts = 2;
    logical is_inverted = TRUE;
    logical is_mirrored = TRUE;
    double  symbol_factor = 0.0;
    double  symbol_anchor[3] = { -1.5, 0.0, 0.0 };
    double  symbol_orient[3] = { 1.5, 0.0, 0.0 };
    double  symbol_angle = 30.0;
    double  symbol_length = 2.0;
    double  symbol_height = 1.0;
    double  point1[3];
    tag_t   point1_tag, point2_tag, point3_tag;
    tag_t   line1_tag, line2_tag;
    tag_t   symbol_tag, symbol_font_tags[9];
    tag_t   dimension_tag;
    char    dimtxt[15 + 1] = "DIMENSION TEXT ";
    char    symbol_file_name[SYMBOL_FILE_NAME_SIZE + 1];
    char    symbol_name[SYMBOL_NAME_SIZE + 1] = "SYMBOLx ";
    UF_CURVE_line_t             line_coords;
    UF_DRF_symbol_create_data_t symbol_data;
    UF_DRF_symbol_data_t        symbol_info;
    UF_DRF_object_t             object1, object2;
    UF_DRF_text_t               drf_text;
    status = UF_initialize();
    if (!status)
    {
        /* create some points and lines */
        point1[0] = 2.0;
        point1[1] = 11.0;
        point1[2] = 0.0;
        UF_CURVE_create_point(point1, &point1_tag);
        point1[0] = 3.0;
        point1[1] = 0.5;
        UF_CURVE_create_point(point1, &point2_tag);
        point1[0] = -0.7;
        point1[1] = 2.5;
        UF_CURVE_create_point(point1, &point3_tag);
        line_coords.start_point[0] = -1.0;
        line_coords.start_point[1] = -1.0;
        line_coords.start_point[2] = 0.0;
        line_coords.end_point[0] = 1.0;
        line_coords.end_point[1] = 1.0;
        line_coords.end_point[2] = 0.0;
        UF_CURVE_create_line(&line_coords, &line1_tag);
        line_coords.start_point[0] = 1.0;
        line_coords.start_point[1] = -1.0;
        line_coords.start_point[2] = 0.0;
        line_coords.end_point[0] = -1.0;
        line_coords.end_point[1] = 1.0;
        line_coords.end_point[2] = 0.0;
        UF_CURVE_create_line(&line_coords, &line2_tag);
        UF_DRF_init_symbol_create_data(&symbol_data);
        /* initialize the object structures */
        UF_DRF_init_object_structure(&object1);
        UF_DRF_init_object_structure(&object2);
        /* load drf_text specification */
        drf_text.user_dim_text = dimtxt;
        drf_text.lines_app_text = 0;
        drf_text.appended_text = NULL;
        /* create a parallel dimension */
        point1[0] = 5.0;
        point1[1] = 3.0;
        object1.object_tag = line1_tag;
        object1.object_view_tag = NULL_TAG;
        object1.object_assoc_type = UF_DRF_end_point;
        object1.object_assoc_modifier = UF_DRF_last_end_point;
        object2.object_tag = point1_tag;
        object2.object_view_tag = NULL_TAG;
        object2.object_assoc_type = UF_DRF_end_point;
        object2.object_assoc_modifier = UF_DRF_first_end_point;
        UF_DRF_create_parallel_dim
        (&object1, &object2, &drf_text, point1,
            &dimension_tag);
        UF_DRF_set_customer_sbf_file();
        UF_DRF_ask_sbf_file(symbol_file_name);
        /* create symbol font entity */
        symbol_font_tags[0] = line1_tag;
        symbol_font_tags[1] = line2_tag;
        UF_DRF_create_symbol_font
        (symbol_name, symbol_factor, symbol_anchor,
            symbol_orient,
            num_symbol_fonts, symbol_font_tags);
        /* create standalone symbol */
        strcpy(symbol_data.symbol_name, symbol_name);
        symbol_data.angle = symbol_angle;
        symbol_data.length = symbol_length;
        symbol_data.height = symbol_height;
        symbol_data.anchor_tag = point1_tag;
        UF_DRF_place_symbol
        (&symbol_data, is_inverted, is_mirrored, &symbol_tag);
        symbol_data.length *= 0.15;
        symbol_data.height *= 0.15;
        symbol_data.anchor_tag = point2_tag;
        UF_DRF_add_symbol_to_object
        (&symbol_data, dimension_tag);
        symbol_data.anchor_tag = point3_tag;
        UF_DRF_add_symbol_to_object
        (&symbol_data, dimension_tag);
        UF_DRF_ask_symbols_used
        (dimension_tag, &num_symbol_fonts, symbol_font_tags);
        status = UF_DRF_ask_symbol_data(symbol_tag, &symbol_info);
        if (status == UF_DRF_NO_ERRORS)
        {
            symbol_font_tags[0] = symbol_info.symbol_font_tag;
            /* absolute anchor point coordinates */
            symbol_anchor[0] = symbol_info.anchor_point[0];
            symbol_anchor[1] = symbol_info.anchor_point[1];
            symbol_anchor[2] = symbol_info.anchor_point[2];
            /* absolute orientation point coordinates */
            symbol_orient[0] = symbol_info.orient_point[0];
            symbol_orient[1] = symbol_info.orient_point[1];
            symbol_orient[2] = symbol_info.orient_point[2];
            /* angle, length, height */
            symbol_angle = symbol_info.angle;
            symbol_length = symbol_info.length;
            symbol_height = symbol_info.height;
        }
    }
}





//===================================================
/**************************************************************************
 *
 * DESCRIPTION:  This function will ask the user for a screen pick and
 *               return the position of the screen pick to the calling
 *               function.  In this example the screen pick is used to
 *               describe a location for the custom symbol.
 *
 ************************************************************************/
static logical specify_position(char* prompt, double pos[3])
{
    int
        resp = uc1615(prompt, pos);

    if (resp == 5) return TRUE;
    else return FALSE;

}

/*****************************************************************************
 *
 * DESCRIPTION:  This routine will create the geometry pieces which will later
 *               be incorporated into a custom symbol.  For the purpose of
 *               this example, we will only include two lines and a circle.
 *               Note, however, that many other objects may be included:
 *               splines, parabolas, hyperbolas, b-curves, crosshatching,
 *               and area fill are all valid consituent geometry for a
 *               custom symbol.
 *
 ****************************************************************************/
static void create_symbol_geometry(UF_DRF_custom_symbol_p_t symbol_definition)
{
    UF_CURVE_line_t
        line_data[2] = { {{ -0.5,0,0 }, { 0.5,0,0}}, {{0,-0.5,0}, {0,0.5,0}} };
    UF_CURVE_arc_t
        arc_data;
    double
        matrix_values[9] = { 1.0, 0.0, 0.0,
                         0.0, 1.0, 0.0,
                            0.0, 0.0, 1.0 },
        arc_center[3] = { 0.0, 0.0, 0.0 };
    tag_t
        matrix_tag = NULL_TAG;
    int irc;

    /*Create space for the tags to be stored*/
    symbol_definition->geometry = (tag_t*)UF_allocate_memory(sizeof(tag_t) * 3, &irc);
    if (irc != 0)
    {
        symbol_definition->geometry = NULL;
        symbol_definition->num_geometry = 0;
        return;
    }
    else
        symbol_definition->num_geometry = 3;

    /*Create the two lines in a vertical "cross" shape*/
    UF_CURVE_create_line(&line_data[0], &(symbol_definition->geometry[0]));
    UF_CURVE_create_line(&line_data[1], &(symbol_definition->geometry[1]));

    /*Create the arc*/
    UF_CSYS_create_matrix(matrix_values, &matrix_tag);
    arc_data.matrix_tag = matrix_tag;
    arc_data.start_angle = 0.0;
    arc_data.end_angle = TWOPI;
    arc_data.arc_center[0] = arc_center[0];
    arc_data.arc_center[1] = arc_center[1];
    arc_data.arc_center[2] = arc_center[2];
    arc_data.radius = 0.5;
    UF_CURVE_create_arc(&arc_data, &(symbol_definition->geometry[2]));
    return;
}

/**********************************************************************
 *
 * DESCRIPTION:  This function will create three points which will be
 *               used as part of the custom symbol definition.
 *               The minimum number of required points for a custom
 *               symbol is one -- the anchor point.  However, you
 *               may also specify left and right leader terminator
 *               points as an option.  If you intend to create a
 *               custom symbol with a leader, you must create at
 *               least one leader terminator point.  If you create
 *               both left and right leader terminator points, the
 *               system will use these points to infer a leader side.
 *
 ********************************************************************/
static void create_symbol_points(UF_DRF_custom_symbol_p_t symbol_definition)
{
    double
        anchor_point[3] = { 0.0, 0.0, 0.0 },
        left_leader_point[3] = { -0.5, 0.0, 0.0 },
        right_leader_point[3] = { 0.5, 0.0, 0.0 };
    UF_DRF_symbol_connection_t
        left_leader_connection,
        right_leader_connection;
    int
        irc;

    /*Create the anchor point*/
    /*Note that the relationship between the anchor point and the rest of
      the symbol geometry at creation time defines the origin of the object.
      When changing the position of the symbol interactively, this point
      will act as the "drag point" around which all the other symbol pieces
      orient themselves.  When changing symbol scale, this point will act
      as an invariant point (unless changing the symbol scale interactively
      via drag handles on the object).*/
    UF_CURVE_create_point(anchor_point, &(symbol_definition->anchor_point));

    /*Create the leader attachment points*/
    symbol_definition->connections = (UF_DRF_symbol_connection_t*)UF_allocate_memory(sizeof(UF_DRF_symbol_connection_t) * 2, &irc);
    if (irc != 0)
    {
        symbol_definition->connections = NULL;
        symbol_definition->num_connections = 0;
        return;
    }
    else
        symbol_definition->num_connections = 2;

    left_leader_connection.connection_type = UF_DRF_LEFT_LEADER_CONNECTION;
    UF_CURVE_create_point(left_leader_point, &(left_leader_connection.connection_point));
    symbol_definition->connections[0] = left_leader_connection;

    right_leader_connection.connection_type = UF_DRF_RIGHT_LEADER_CONNECTION;
    UF_CURVE_create_point(right_leader_point, &(right_leader_connection.connection_point));
    symbol_definition->connections[1] = right_leader_connection;
    return;
}

/************************************************************************
 *
 * DESCRIPTION: This function will create all the note objects necessary
 *              to define the text fields in the custom symbol.  For the
 *              purposes of this example, we will create one mandatory
 *              piece of text and one controlled piece of text.  In
 *              general, however, you may create as many notes as you want
 *              using the following types:
 *                Mandatory:  The text cannot be edited
 *                Controlled: The text may only be edited from a list of
 *                            predefined strings.
 *                Partially controlled:  The text may be edited from a list
 *                                       of predefined strings, or it can
 *                                       be arbitrary.
 *                Arbitrary: The text may be edited arbitrarily
 *                Integer: The text may only be an integer number within
 *                         certain boundaries.
 *                Real: The text may only be a real number within certain
 *                      boundaries.
 *
 *************************************************************************/
static void create_symbol_text(UF_DRF_custom_symbol_p_t symbol_definition)
{
    int
        integer_draft_prefs[100],
        original_integer_draft_prefs[100],
        num_strings = 5,
        inx = 0,
        irc;
    double
        real_draft_prefs[70],
        original_real_draft_prefs[70],
        upper_right_note_origin[3] = { 0.125, 0.125, 0.0 },
        lower_left_note_origin[3] = { -0.125, -0.125, 0.0 };
    char
        diameter[27],
        radius[27],
        * note_text[1] = { const_cast<char*>("DUMMY TEXT") },
        * mandatory_note_title = const_cast<char*>("My mandatory note"),
        * mandatory_text_string = const_cast<char*>("AA"),
        * controlled_note_title = const_cast<char*>("My controlled note"),
        * controlled_note_strings[5] = {    const_cast<char*>("BB"), 
                                            const_cast<char*>("CC"), 
                                            const_cast<char*>("DD"), 
                                            const_cast<char*>("EE"), 
                                            const_cast<char*>("FF") };
    tag_t
        upper_right_note,
        lower_left_note;
    UF_DRF_custom_symbol_text_t
        mandatory_note,
        controlled_note;


    /*Set up the preferences that we will use to create the notes.  Also,
      save a copy of the original preferences so that we can change back
      to the original preferences after creating the notes.*/
    UF_DRF_ask_preferences(integer_draft_prefs, real_draft_prefs,
        radius, diameter);
    UF_DRF_ask_preferences(original_integer_draft_prefs,
        original_real_draft_prefs, radius,
        diameter);
    /*Set the entity site to be lower left.  This will enable the note in
      the upper right portion of the symbol to grow up and to the right
      when the text size changes.*/
    integer_draft_prefs[30] = 7;

    /*Save the changes to drafting preferences.*/
    /*Although for this example we are only changing the text alignment
      position, notice that the text objects in the symbol definition
      will display using all of the preferences of the note at the time
      of symbol creation.  Thus, if you are interested in maintaining
      text at a certain size, font, or color different from the current
      drafting preferences, make sure the drafting preferences you
      desire are applied to the notes before the custom symbol is created.*/
    UF_DRF_set_preferences(integer_draft_prefs, real_draft_prefs,
        radius, diameter);

    /*Create the notes.  Be aware that the system will use the note origin
      with respect to the other symbol pieces to define the shape of the
      symbol.  Also be aware that the text alignment position of the note
      will affect how the note moves when the text size is changed.  The
      text alignment position will be an invariant point with respect to
      the rest of the symbol pieces if the text size changes.*/
      /*The first note we create will be located in the upper right quadrant,
        with its lower left point slightly above and to the right of the
        symbol origin*/
    UF_DRF_create_note(1, note_text, upper_right_note_origin, 0,
        &upper_right_note);

    /*Change the text alignment location for the second note:
      Set the entity site to be upper right.  This will enable the note
      in the lower left portion of the symbol to grow down and to the
      left when the text size changes.*/
    integer_draft_prefs[30] = 3;
    /*Save the changes to drafting preferences.*/
    UF_DRF_set_preferences(integer_draft_prefs, real_draft_prefs,
        radius, diameter);

    /*Create the second note.  This note will be located with its upper
      right corner slightly below and to the left of the symbol origin.*/
    __debugbreak;
        UF_DRF_create_note(1, note_text, lower_left_note_origin, 0,
        &lower_left_note);

    /*Create the text objects.  Now that the note objects have been
      created, we can specify note type information as well as the
      predefined text strings for controlled text types or upper and lower
      bounds for numerical text types.  In this example we will use the
      upper right note to create a mandatory text object with the text
      "AA" and use the lower left note to create a controlled text object
      with the default text "BB", but also define the text strings "CC",
      "DD", "EE", and "FF" as valid choices.*/
    UF_DRF_initialize_custom_symbol_text_data(&mandatory_note);
    mandatory_note.note_tag = upper_right_note;
    mandatory_note.text_type = UF_DRF_MANDATORY_TEXT;
    mandatory_note.title = mandatory_note_title;
    mandatory_note.simple_text = mandatory_text_string;

    UF_DRF_initialize_custom_symbol_text_data(&controlled_note);
    controlled_note.note_tag = lower_left_note;
    controlled_note.text_type = UF_DRF_FULLY_CONTROLLED_TEXT;
    controlled_note.title = controlled_note_title;
    controlled_note.num_optional_strings = num_strings;
    controlled_note.optional_strings = (char**)UF_allocate_memory((sizeof(char*) * num_strings), &irc);
    if (irc != 0)
        return;
    for (inx = 0; inx < num_strings; inx++)
    {
        controlled_note.optional_strings[inx] = controlled_note_strings[inx];
    }
    controlled_note.current_option = 0;

    /*Assign the newly created text objects to the symbol definition
      structure*/
    symbol_definition->num_text = 2;
    symbol_definition->text = (UF_DRF_custom_symbol_text_t*)UF_allocate_memory(sizeof(UF_DRF_custom_symbol_text_p_t) * 2, &irc);
    if (irc != 0)
        return;
    symbol_definition->text[0] = mandatory_note;
    symbol_definition->text[1] = controlled_note;

    /*Reset drafting preferences since we are done creating the notes*/
    UF_DRF_set_preferences(original_integer_draft_prefs,
        original_real_draft_prefs, radius,
        diameter);
}

/************************************************************************
 *
 * DESCRIPTION:  This function will initialize the data structure and
 *               set the appropriate information to add a leader to the
 *               custom symbol.
 *
 ***********************************************************************/
static void create_symbol_leader(UF_DRF_custom_symbol_p_t symbol_definition)
{
    UF_DRF_leader_p_t leader;
    double leader_terminator_point_coords[3] = { -4.0, -3.0, 0.0 };
    int irc;
    UF_UI_write_listing_window(const_cast<char*>("1\n"));
    symbol_definition->leader = (UF_DRF_leader_p_t)UF_allocate_memory(sizeof(UF_DRF_leader_t), &irc);
    if (irc != 0)
    {
        symbol_definition->leader = NULL;
        return;
    }
    else leader = symbol_definition->leader;

    /*We can add more than one leader to a single leader structure.
      However, for purposes of this example we will only use one.*/
    leader->num_leaders = 1;

    /*Create the terminator point for the leader and add it to the
      structure*/
    leader->leader_terminators = (tag_t*)UF_allocate_memory(sizeof(tag_t), &irc);
    if (irc != 0)
        return;
    UF_UI_write_listing_window(const_cast<char*>("2\n"));
    UF_CURVE_create_point(leader_terminator_point_coords,
        &(leader->leader_terminators[0]));

    //Define the leader to be from the left.
    leader->leader_orientation = UF_DRF_LEADER_LEFT;

    /*Create an intermediate points structure.  This structure contains
      a list of point coordinates for any intermediate points between
      the leader terminator and the stub.  In this example, there are
      no intermediate points.*/
    UF_UI_write_listing_window(const_cast<char*>("3\n"));
    leader->intermediate_points = (UF_DRF_intermediate_points_t*)UF_allocate_memory(sizeof(UF_DRF_intermediate_points_t), &irc);
    if (irc != 0) return;
    leader->intermediate_points[0].num_points = 0;
    leader->intermediate_points[0].points = NULL;
    UF_UI_write_listing_window(const_cast<char*>("4\n"));
    return;
}

static void clean_up_memory(UF_DRF_custom_symbol_p_t symbol_definition)
{
    int
        inx = 0;

    /*Clean up the geometry array*/
    UF_free(symbol_definition->geometry);

    /*Clean up the connection array*/
    UF_free(symbol_definition->connections);

    /*Clean up the text array*/
    UF_free(symbol_definition->text->optional_strings);
    UF_free(symbol_definition->text);

    /*Clean up the leader data*/
    for (inx = 0; inx < symbol_definition->leader->num_leaders; inx++)
    {
        UF_free(symbol_definition->leader->leader_terminators);

        UF_free(symbol_definition->leader->intermediate_points[inx].points);
    }
    UF_free(symbol_definition->leader->intermediate_points);
    UF_free(symbol_definition->leader);

}

/********************************************************************
 *
 * DESCRIPTION:  This function is the driving function for the
 *               creation of a custom symbol.  This function will
 *               initialize the data, fill in the appropriate data
 *               structures, and pass the data into the API to create
 *               the custom symbol.
 *
 * NOTES:
 * 1) The API currently does not support the creation of custom symbol
 *    instances from master symbol definitions.  This demonstration
 *    program will only show how to create individual custom symbol
 *    instances from non-reusable sets of geometry.
 *
 * 2) The API currently does not support editing of Custom Symbols
 *    similar to the functionality provided by selecting a Custom
 *    Symbol in the Insert->Custom Symbol dialog in Drafting.
 *    Until support is added in UG/Open, the only way to edit Custom
 *    Symbols (apart from editing symbol scale, angle, color,
 *    line font, and line width) is to edit them interactively.
 *    The ability to switch between a predefined set of text strings
 *    with controlled notes is one of the types of edits that is only
 *    available interactively.
 *
 ********************************************************************/
static void create_custom_symbol(void)
{
    UF_UI_open_listing_window();

    


    tag_t
        symbol;
    UF_DRF_custom_symbol_t
        symbol_definition;

    /*Initialize the custom symbol data structures.*/
    UF_DRF_initialize_custom_symbol_data(&symbol_definition);

    UF_UI_write_listing_window(const_cast<char*>("create_symbol_geometry\n"));
    /*Create the geometry objects which will constitute the custom symbol.*/
    create_symbol_geometry(&symbol_definition);

    UF_UI_write_listing_window(const_cast<char*>("create_symbol_points\n"));
    /*Create the points of the symbol. (Includes the symbol anchor point
      and the leader attachment points)*/
    create_symbol_points(&symbol_definition);

    UF_UI_write_listing_window(const_cast<char*>("create_symbol_text\n"));
    /*Create the symbol text objects.*/
    create_symbol_text(&symbol_definition);

    UF_UI_write_listing_window(const_cast<char*>("create_symbol_leader\n"));
    /*Create the symbol leader*/
    create_symbol_leader(&symbol_definition);

    UF_UI_write_listing_window(const_cast<char*>("specify_position\n"));
    /*Get the actual location for the symbol from a screen pick*/
    specify_position(const_cast<char*>("Place custom symbol"), symbol_definition.origin);

    UF_UI_write_listing_window(const_cast<char*>("UF_DRF_create_custom_symbol_instance\n"));
    /*Create the custom symbol object*/
    UF_DRF_create_custom_symbol_instance(&symbol_definition, &symbol);

    UF_UI_write_listing_window(const_cast<char*>("clean_up_memory\n"));
    /*Clean up dynamically allocated memory*/
    clean_up_memory(&symbol_definition);
}

