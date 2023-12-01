#include "callout.hpp"

typedef struct
{
    UF_CURVE_arc_t* arc;
    double          abs_ctr[3];
    double          matrix[9];
    double          x_axis[3];
    double          y_axis[3];
} my_motion_data_t;
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
    /* Calculate the arc radius:  the distance from the arc center
     * to the current screen position.
     */
    UF_VEC3_distance(my_data->abs_ctr, screen_pos, &radius);
    /* Map the arc center to the "csys of the arc".
     */
    UF_MTX3_vec_multiply(my_data->abs_ctr,
        my_data->matrix,
        my_data->arc->arc_center);
    /* Draw a circle and an arc in the view of the cursor.
     */
    UF_DISP_display_ogp_circle(motion_cb_data->view_tag,
        my_data->matrix,
        my_data->arc->arc_center,
        radius);
    UF_DISP_display_ogp_arc(motion_cb_data->view_tag,
        my_data->matrix,
        15.0 * DEGRA, 345.0 * DEGRA,
        my_data->arc->arc_center,
        1.0);
    /* Draw a bounding box around the circle.
     */
    UF_VEC3_scale(radius, my_data->x_axis, xrad);
    UF_VEC3_scale(radius, my_data->y_axis, yrad);
    UF_VEC3_add(xrad, yrad, x_y_vec);
    UF_VEC3_sub(xrad, yrad, x_ny_vec);
    UF_VEC3_add(my_data->abs_ctr, x_y_vec, pos_array[0]);
    UF_VEC3_sub(my_data->abs_ctr, x_ny_vec, pos_array[1]);
    UF_VEC3_sub(my_data->abs_ctr, x_y_vec, pos_array[2]);
    UF_VEC3_add(my_data->abs_ctr, x_ny_vec, pos_array[3]);
    UF_VEC3_add(my_data->abs_ctr, x_y_vec, pos_array[4]);
    UF_DISP_display_ogp_polyline(motion_cb_data->view_tag,
        pos_array, 5);
    /* Draw a "rubberband" line from the circle center to the
     * cursor position.
     */
    UF_DISP_display_ogp_line(motion_cb_data->view_tag,
        my_data->abs_ctr,
        screen_pos);
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




static void do_ugopen_api(void)
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
    do
    {
        /* Specify the plane on which the circle is to be created.
         */
        UF_UI_specify_plane(
            const_cast<char*>("Specify plane for circle creation"),
            &default_plane,
            1,
            &plane_resp,
            plane_matrix,
            plane_origin,
            &plane_tag);
        if (plane_resp == 3)
        {
            /* Save the current WCS for future restoration.
             */
            UF_CSYS_ask_wcs(&saved_wcs);
            /* Move the WCS to the specified plane. This is
             * necessary because the position passed to the motion
             * callback, and the position returned by
             * UF_UI_specify_screen_position, is the screen
             * position projected onto the WCS XY plane.
             */
            if (default_plane != 5)
            {
                UF_CSYS_create_matrix(plane_matrix, &mtx_id);
                UF_CSYS_create_csys(plane_origin, mtx_id,
                    &csys_id);
                UF_CSYS_set_wcs(csys_id);
            }
            /* Obtain unit vectors and the arc matrix relative to
             * the Work Part coordinate system.
             */
            UF_MTX3_x_vec(plane_matrix, x_vec);
            UF_MTX3_y_vec(plane_matrix, y_vec);
            UF_CSYS_map_point(UF_CSYS_ROOT_COORDS, x_vec,
                UF_CSYS_WORK_COORDS, x_vec);
            UF_CSYS_map_point(UF_CSYS_ROOT_COORDS, y_vec,
                UF_CSYS_WORK_COORDS, y_vec);
            UF_CSYS_map_point(UF_CSYS_ROOT_COORDS, root_origin,
                UF_CSYS_WORK_COORDS, work_origin);
            UF_VEC3_sub(x_vec, work_origin, my_data.x_axis);
            UF_VEC3_sub(y_vec, work_origin, my_data.y_axis);
            UF_MTX3_initialize(my_data.x_axis, my_data.y_axis,
                my_data.matrix);
            UF_CSYS_create_matrix(my_data.matrix,
                &arc.matrix_tag);
            do
            {
                UF_UI_specify_screen_position(
                    const_cast<char*>("Specify arc center"),
                    NULL,
                    NULL,
                    my_data.abs_ctr,
                    &view_tag,
                    &ctr_resp);
                if (ctr_resp == UF_UI_PICK_RESPONSE)
                {
                    /* Map the arc center to the arc csys.
                     */
                    UF_MTX3_vec_multiply(my_data.abs_ctr,
                        my_data.matrix,
                        arc.arc_center);
                    UF_UI_specify_screen_position(
                        const_cast<char*>("Indicate arc radius"),
                        (UF_UI_motion_fn_t)motion_cb,
                        (void*)&my_data,
                        arc_edge_pos,
                        &view_tag,
                        &pos_resp);
                    /* If a position was obtained, create the
                     * circle.
                     */
                    if (pos_resp == UF_UI_PICK_RESPONSE)
                    {
                        UF_VEC3_distance(my_data.abs_ctr,
                            arc_edge_pos,
                            &arc.radius);
                        UF_CURVE_create_arc(&arc, &arc_id);
                    }
                }
            } while ((ctr_resp == UF_UI_PICK_RESPONSE) &&
                (pos_resp != UF_UI_CANCEL));
            /* Restore the WCS if it was altered above.
             */
            if (default_plane != 5)
            {
                UF_CSYS_set_wcs(saved_wcs);
                UF_OBJ_delete_object(csys_id);
            }
        }
    } while (ctr_resp == UF_UI_BACK && plane_resp == 3);
}










/* ѕроцедура вычерчивани€ габаритного Ѕќ —а временными лини€ми

 Ќа входе:

min_corner Ч координаты начальной точки Ѕќ —а

directions Ч массив направлений трех базовых ребер Ѕќ —а

distances Ч  массив длин базовых ребер Ѕќ —а

*/
/*
void BoundBox(double min_corner[3], double directions[3][3], double distances[3])
{
    double p_N[10][3];  //массив точек углов Ѕќ —а
    int i, j, k;  //индексы циклов
    tag_t work_view;  //тэг области просмотра
  //структура свойств области просмотра
    UF_OBJ_disp_props_t props;
    //заполнить структуру свойств области просмотра объекта текущими данными
    UF_OBJ_ask_display_properties(object, &props);
    //изменить цвет отображени€ временных линий на Ђбурыйї
    props.color = 148;
    //получить тэг области просмотра
    UF_VIEW_ask_work_view(&work_view);
    //прочертить базовые три ребра Ѕќ —ј
    for (i = 0; i < 3; i++)
    { //дл€ этого вычислить координаты конечной точки текущего ребра
        UF_VEC3_affine_comb(min_corner, distances[i],
            directions[i], p_N[i]);
        //чертить текущее ребро от начальной точки до конечной (расчетной)
        UF_DISP_display_temporary_line(work_view,
            UF_DISP_USE_WORK_VIEW, min_corner, p_N[i], &props);
    }
    //из концов построенных ребер строим еще ребра (по два)
    k = 3;
    for (j = 0; j < 3; j++)
        for (i = 0; i < 3; i++)
            if (j != i) {/* если индекс ребра и индекс направлени€
             совпадают, такое ребро не строим */
/*                UF_VEC3_affine_comb(p_N[j], distances[i],
                    directions[i], p_N[k]);
                UF_DISP_display_temporary_line(work_view,
                    UF_DISP_USE_WORK_VIEW, p_N[j], p_N[k], &props);
                k++;
            }
    //рассчитываем координаты последнего, самого удаленного узла
    UF_VEC3_affine_comb(p_N[7], distances[1],
        directions[1], p_N[k]);
    //и дочерчиваем оставшиес€ три ребра
    UF_DISP_display_temporary_line(work_view,
        UF_DISP_USE_WORK_VIEW, p_N[7], p_N[k], &props);
    UF_DISP_display_temporary_line(work_view,
        UF_DISP_USE_WORK_VIEW, p_N[k], p_N[3], &props);
    UF_DISP_display_temporary_line(work_view,
        UF_DISP_USE_WORK_VIEW, p_N[k], p_N[6], &props);
}*/

/*
//ƒо создани€ текста установим высоту и шрифт:
//” чертежа есть массивы свойств:
int mpi[100];
double mpr[70];  char radius_value[27];
char diameter_value[27];
UF_CALL(UF_DRF_ask_preferences(mpi, mpr, radius_value, diameter_value)); //считаем массивы свойств
mpr[44] = 3.5; //изменим высоту шрифта
mpi[88] = get_font(part_tag, "gost2.304_81");////шрифт установим
UF_CALL(UF_DRF_set_preferences(mpi, mpr, radius_value, diameter_value)); // изменим массив...
//возвращает номер шрифта в таблице шрифтов по его имени и устанавливает шрифт
//tag_t part_tag - тэг части
//string name    - им€ шрифта
int get_font(tag_t part_tag, char* name)
{
    int num = 0;
    tag_t fte_tag = NULL;
    UF_OBJ_cycle_objs_in_part(part_tag, UF_font_table_type, &fte_tag);
    if (fte_tag)
    {
        UF_CALL(UF_UGFONT_add_font(fte_tag, &num, name)); //добавл€ет шрифт в таблицу
    }
    return num;
}*/

//===================================================================================
static void create_symbol_leader(UF_DRF_custom_symbol_p_t symbol_definition)
{
    UF_DRF_leader_p_t leader;
    double leader_terminator_point_coords[3] = { -4.0, -3.0, 0.0 };
    int irc;

    symbol_definition->leader = (UF_DRF_leader_p_t)UF_allocate_memory(sizeof(UF_DRF_leader_t), &irc);
    if (irc != 0)
    {
        symbol_definition->leader = NULL;
        return;
    }
    else
        leader = symbol_definition->leader;

    //We can add more than one leader to a single leader structure.
    // However, for purposes of this example we will only use one.
    leader->num_leaders = 1;

    //Create the terminator point for the leader and add it to the structure
    leader->leader_terminators = (tag_t*)UF_allocate_memory(sizeof(tag_t), &irc);
    if (irc != 0)
        return;
    UF_CURVE_create_point(leader_terminator_point_coords,
        &(leader->leader_terminators[0]));

    //Define the leader to be from the left.
    leader->leader_orientation = UF_DRF_LEADER_LEFT;

    /*Create an intermediate points structure.  This structure contains
      a list of point coordinates for any intermediate points between
      the leader terminator and the stub.  In this example, there are
      no intermediate points.*/
    leader->intermediate_points = (UF_DRF_intermediate_points_t*)UF_allocate_memory(sizeof(UF_DRF_intermediate_points_t), &irc);
    if (irc != 0)
        return;
    leader->intermediate_points[0].num_points = 0;
    leader->intermediate_points[0].points = NULL;

    return;
}