cxx_test(
   TARGET euclidean_vector_test1
   FILENAME "euclidean_vector_test1.cpp"
   LINK euclidean_vector
)

cxx_test(
   TARGET euclidean_vector_test1_constructor
   FILENAME "euclidean_vector_test1_constructor.cpp"
   LINK euclidean_vector
)

cxx_test(
   TARGET euclidean_vector_test2_operations
   FILENAME "euclidean_vector_test2_operations.cpp"
   LINK euclidean_vector
)

cxx_test(
   TARGET euclidean_vector_test3_members
   FILENAME "euclidean_vector_test3_members.cpp"
   LINK euclidean_vector
)

cxx_test(
   TARGET euclidean_vector_test4_friends
   FILENAME "euclidean_vector_test4_friends.cpp"
   LINK euclidean_vector
)

cxx_test(
   TARGET euclidean_vector_test5_utility
   FILENAME "euclidean_vector_test5_utility.cpp"
   LINK euclidean_vector
)

cxx_test(
   TARGET euclidean_vector_test6_integration
   FILENAME "euclidean_vector_test6_integration.cpp"
   LINK euclidean_vector
)