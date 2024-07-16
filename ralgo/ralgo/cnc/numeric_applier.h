

// class control_block
// {
//     struct dlist_head lnk;

//     int32_t steps[N_AXES];

//     int32_t major_step;
//     int32_t acceleration_before;
//     int32_t deceleration_after;

//     float nominal_increment;
//     float acceleration;
//     float deceleration;

//     float multipliers[N_AXES];
//     float accelerations[N_AXES];
//     float decelerations[N_AXES];

//     int64_t acceleration_before_ic;
//     int64_t deaceleration_after_ic;
//     int64_t block_finish_ic;

//     uint8_t exact_stop;

//     bool is_active(int64_t interrupt_counter)
//     {
//         if (exact_stop)
//             return interrupt_counter < block_finish_ic;
//         else
//             return interrupt_counter < deaceleration_after_ic;
//     }

//     bool is_postactive()
//     {
//         return interrupt_counter < block_finish_ic;
//     }

//     float current_acceleration(int64_t interrupt_counter)
//     {
//         if (interrupt_counter < acceleration_before_ic)
//             return acceleration;
//         if (interrupt_counter < deaceleration_after_ic)
//             return 0;
//         return deceleration;
//     }

//     // runtime
//     uint8_t state;
// }

// class control_axis_phase
// {
//     int64_t steps;
//     float dda_counter;
//     float velocity;
//     float acceleration;
// }

// class planned_blocks_ring
// {
//     int64_t interrupt_counter;
//     int64_t revolver_iteration_counter;

//     int64_t reference_position[N_AXES];

//     float delta;
//     float delta_sqr_div_2;

//     control_block *active_block;
//     dlist_head postactive_blocks;

//     ring_head ring;
//     int ring_postactive_head;

//     control_block planned[PLANNED_BLOCKS_RING];

//     int change_active_block()
//     {
//         ring_move_head_one(&ring);
//     }

//     int block_index(control_block *it)
//     {
//         return it - planned;
//     }

//     void fixup_postactive_blocks()
//     {
//         while (ring_postactive_counter_head != ring->head)
//         {
//             if (!planned[ring_postactive_counter_head].is_postactive())
//             {
//                 ring_postactive_head = (ring_postactive_head + 1) %
//                 ring->size; planned[ring_postactive_counter_head].valid =
//                 false;
//             }

//             else
//             {
//                 break;
//             }
//         }
//     }

//     void discard_postactive(control_block *it)
//     {
//         if (block_index(it) == ring_postactive_counter_head)
//             fixup_postactive_blocks();
//     }

//     void algorithm_step_for_trapecidal_profile()
//     {
//         int final;
//         int room = revolver_cycle->room();

//         while (room--)
//         {
//             // Планируем поведение револьвера на несколько циклов вперёд
//             // попутно инкрементируя модельное время.
//             final = iteration(revolver_iteration_counter);

//             if (final)
//                 return final;

//             ++revolver_iteration_counter;
//         }
//     }

//     int iteration()
//     {
//         control_block *itblock;

//         if (!active_block->is_active(current_iteration))
//         {
//             int final = change_active_block(active_block);

//             if (final)
//                 return final;
//         }

//         auto acceleration = active_block->current_acceleration();

//         dlist_for_each_entry_safe(itblock, postactive_blocks, lnk)
//         {
//             if (!itblock->is_postactive())
//                 discard_postactive(itblock);

//             acceleration += itblock->current_acceleration();
//         }

//         for (int i = 0; i < total_axes; ++i)
//         {
//             axes_records[i]->dda_counter +=
//                 axes_records[i]->velocity * delta +
//                 axes_records[i]->acceleration * delta_sqr_div_2;

//             if (axes_records[i]->dda_counter > 1)
//             {
//                 axes_records[i]->dda_counter -= 1;
//                 axes_records[i]->steps += 1;
//             }
//             else if (axes_records[i]->dda_counter < -1)
//             {
//                 axes_records[i]->dda_counter += 1;
//                 axes_records[i]->steps -= 1;
//             }

//             axes_records[i]->velocity += axes_records[i]->acceleration *
//             delta;
//         }
//     }

//     class revolver_ring
//     {
//         typedef uint16_t revolver_t;

//     private:
//         struct revolver_shift
//         {
//             revolver_t step;
//             revolver_t direction;
//         };

//     private:
//         int64_t iteration_counter;
//         ring_head ring;

//         int ring_size;
//         revolver_shift *shifts;

//     public:
//         revolver_ring(revolver_shift *shifts, int ring_size) shifts(shifts),
//             ring_size(ring_size)
//         {
//         }

//         void room()
//         {
//             // Операция взятия оставшегося места над кольцевым буфером
//             // требует сравнения head и tail, изменяемых в разных потоках,
//             // поэтому не является атомарной.
//             system_lock();
//             int ret = ring_room(&ring);
//             system_unlock();

//             return ret;
//         }

//         void push(uint16_t step, uint16_t dir)
//         {
//             // Добавление данных в очередь не требует блокировки,
//             // потому что ring_head lockfree на добавление и чтение,
//             // если unsigned int атомарен.

//             int idx = ring_head_index(&ring);
//             shifts[idx] = step;
//             shifts[idx] = direction;
//             ring_move_head_one(&ring);
//         }

//         void serve()
//         {
//             // В общем случае ring_empty не атомарен. Однако, здесь должен
//             // быть контекст приоритетного прерывания.
//             if (ring_empty(&ring))
//                 return;

//             int idx = ring_index(&ring);
//             auto &shift = shifts[idx];

//             for (int i = 0; i < axes_total; ++i)
//             {
//                 revolver_t mask = 1 << i;
//                 bool step = shift.step & mask;

//                 if (!step)
//                     continue;

//                 bool dir = shift.direction & mask;
//                 switch (dir)
//                 {
//                 case -1:
//                     axes[i].dec();
//                     break;
//                 case 1:
//                     axes[i].inc();
//                     break;
//                 default:
//                     break;
//                 }
//             }

//             ring_move_tail_one(&ring);
//             return;
//         }
//     }
// }
