import java.util.*;
import java.lang.management.ManagementFactory;
import java.lang.management.MemoryMXBean;

public class AnchorList_vs_vector_half_remove {
        static int[] values;
        static int[] indexes;
        static int[] deleteIndexes;
        static long startTime, endTime;
        static Double memoryUsed;

        private static double getStableHeapUsedKB() {
                MemoryMXBean bean = ManagementFactory.getMemoryMXBean();
                long last = -1, cur = -2;
                int attempts = 5;
                while (attempts-- > 0 && cur != last) {
                        last = cur;
                        System.gc();
                        bean.gc();
                        try {
                                Thread.sleep(50);
                        } catch (InterruptedException ignored) {
                        }
                        cur = bean.getHeapMemoryUsage().getUsed();
                }
                return cur / 1024.0;
        }

        private static void insertToAnchorList(AnchorList<Integer> ell, int size,
                        Map<Integer, Double> timeTakenMap, Map<Integer, Double> memoryUsed_map)
                        throws IllegalAccessException {

                double startMemory = getStableHeapUsedKB();
                long startTime = System.nanoTime();
                for (int i = 0; i < size; i++) {
                        // System.out.println("index: " + indexes[i] + "\t value:" + values[i]);
                        ell.insert(indexes[i], values[i]);
                        // ell.drawTree();
                        // ell.iterateInOrder();
                        // System.out.println("===============");
                }
                long endTime = System.nanoTime();

                double endMemory = getStableHeapUsedKB();
                memoryUsed = endMemory - startMemory;

                Double timeTaken = ((endTime - startTime) / 1_000_000_000.0); // seconds
                if (timeTakenMap != null) {
                        timeTakenMap.put(size, timeTaken);
                }

                if (memoryUsed_map != null) {
                        memoryUsed_map.put(size, memoryUsed);
                }
                // System.out.println("insert time taken anchor list: " + timeTaken);
        }

        private static void insertToVector(ArrayList<Integer> vec, int size, Map<Integer, Double> timeTakenMap,
                        Map<Integer, Double> memoryUsed_map) {
                double startMemory = getStableHeapUsedKB();

                long startTime = System.nanoTime();
                for (int i = 0; i < size; i++) {
                        vec.add(indexes[i], values[i]);
                }
                long endTime = System.nanoTime();

                double endMemory = getStableHeapUsedKB();
                memoryUsed = endMemory - startMemory;

                Double timeTaken = ((endTime - startTime) / 1_000_000_000.0); // seconds
                if (timeTakenMap != null) {
                        timeTakenMap.put(size, timeTaken);
                }
                if (memoryUsed_map != null) {
                        memoryUsed_map.put(size, memoryUsed);
                }
                // System.out.println("insert time taken vector: " + timeTaken);
        }

        private static boolean valueCheck(
                        AnchorList<Integer> ell,
                        ArrayList<Integer> vec,
                        int size,
                        Map<Integer, Double> ellTimeTakenMap,
                        Map<Integer, Double> vecTimeTakenMap) throws IllegalAccessException {
                boolean result = true;
                int ellValue, vectorValue;

                // Check for value equality
                for (int i = 0; i < size; i++) {
                        if (!ell.get(i).equals(vec.get(i))) {
                                // System.out.println(ell.get(i) + " " + vec.get(i));
                                result = false;
                                break;
                        }
                }

                // Measure AnchorList get time
                long startTime = System.nanoTime();
                for (int i = 0; i < size; i++) {
                        ellValue = ell.get(i);
                }
                long endTime = System.nanoTime();
                Double ellTimeTaken = ((endTime - startTime) / 1_000_000_000.0);

                // Measure ArrayList get time
                startTime = System.nanoTime();
                for (int i = 0; i < size; i++) {
                        vectorValue = vec.get(i);
                }
                endTime = System.nanoTime();
                Double vectorTimeTaken = ((endTime - startTime) / 1_000_000_000.0);

                // Store timings in maps if provided
                if (ellTimeTakenMap != null) {
                        ellTimeTakenMap.put(size, ellTimeTaken);
                }
                if (vecTimeTakenMap != null) {
                        vecTimeTakenMap.put(size, vectorTimeTaken);
                }

                // Optionally print timings
                // System.out.println("ell get time taken: " + ellTimeTaken);
                // System.out.println("vector get time taken: " + vectorTimeTaken);

                return result;
        }

        private static void removeFromAnchorList(AnchorList<Integer> ell, int deleteIndexesSize,
                        Map<Integer, Double> timeTakenMap, Map<Integer, Double> memoryUsed_map)
                        throws IllegalAccessException {
                double startMemory = getStableHeapUsedKB();
                long startTime = System.nanoTime();
                for (int i = 0; i < deleteIndexesSize; i++) {
                        ell.remove(deleteIndexes[i]);
                }
                long endTime = System.nanoTime();

                System.gc();

                double endMemory = getStableHeapUsedKB();
                memoryUsed = startMemory - endMemory;

                Double timeTaken = ((endTime - startTime) / 1_000_000_000.0); // seconds
                if (timeTakenMap != null) {
                        timeTakenMap.put(deleteIndexesSize, timeTaken);
                }
                if (memoryUsed_map != null) {
                        memoryUsed_map.put(deleteIndexesSize, memoryUsed);
                }
                // System.out.println("remove time taken anchor list: " + timeTaken);
        }

        private static void removeFromVector(ArrayList<Integer> vec, int deleteIndexesSize,
                        Map<Integer, Double> timeTakenMap, Map<Integer, Double> memoryUsed_map) {
                double startMemory = getStableHeapUsedKB();

                long startTime = System.nanoTime();
                for (int i = 0; i < deleteIndexesSize; i++) {
                        vec.remove(deleteIndexes[i]);
                }
                long endTime = System.nanoTime();

                vec.trimToSize();
                System.gc();

                double endMemory = getStableHeapUsedKB();
                memoryUsed = startMemory - endMemory;

                Double timeTaken = ((endTime - startTime) / 1_000_000_000.0); // seconds
                if (timeTakenMap != null) {
                        timeTakenMap.put(deleteIndexesSize, timeTaken);
                }
                if (memoryUsed_map != null) {
                        memoryUsed_map.put(deleteIndexesSize, memoryUsed);
                }
                // System.out.println("remove time taken vector: " + timeTaken);
        }

        public static void saveMapToCSV(Map<Integer, Double> data, String filename) {
                String fullPath = "/home/sepehr/uni/DS/paper/AnchorList_JAVA/" + filename;
                try (java.io.PrintWriter file = new java.io.PrintWriter(fullPath)) {
                        file.println("Size,Time");
                        java.text.DecimalFormat df = new java.text.DecimalFormat("0.000000000");
                        for (Map.Entry<Integer, Double> entry : data.entrySet()) {
                                file.println(entry.getKey() + "," + df.format(entry.getValue()));
                        }
                        System.out.println("Saved " + fullPath);
                } catch (java.io.IOException e) {
                        System.out.println("Error: Could not create " + fullPath);
                        System.out.println("Make sure the results directory exists");
                }
        }

        public static void main(String[] args) throws IllegalAccessException {

                // memory consumption
                Map<Integer, Double> pushBack_ell_memoryResult = new HashMap<>();
                Map<Integer, Double> pushBack_vec_memoryResult = new HashMap<>();
                Map<Integer, Double> popBackAfterPushBack_ell_memoryResult = new HashMap<>();
                Map<Integer, Double> popBackAfterPushBack_vec_memoryResult = new HashMap<>();
                Map<Integer, Double> popFrontAfterPushBack_ell_memoryResult = new HashMap<>();
                Map<Integer, Double> popFrontAfterPushBack_vec_memoryResult = new HashMap<>();
                Map<Integer, Double> removeRandomIndicesAfterPushBack_ell_memoryResult = new HashMap<>();
                Map<Integer, Double> removeRandomIndicesAfterPushBack_vec_memoryResult = new HashMap<>();

                Map<Integer, Double> pushFront_ell_memoryResult = new HashMap<>();
                Map<Integer, Double> pushFront_vec_memoryResult = new HashMap<>();
                Map<Integer, Double> popFrontAfterPushFront_ell_memoryResult = new HashMap<>();
                Map<Integer, Double> popFrontAfterPushFront_vec_memoryResult = new HashMap<>();
                Map<Integer, Double> popBackAfterPushFront_ell_memoryResult = new HashMap<>();
                Map<Integer, Double> popBackAfterPushFront_vec_memoryResult = new HashMap<>();

                Map<Integer, Double> insertRandomIndices_ell_memoryResult = new HashMap<>();
                Map<Integer, Double> insertRandomIndices_vec_memoryResult = new HashMap<>();
                Map<Integer, Double> removeRandomIndices_ell_memoryResult = new HashMap<>();
                Map<Integer, Double> removeRandomIndices_vec_memoryResult = new HashMap<>();

                Map<Integer, Double> pushFront_pushBack_ell_memoryResult = new HashMap<>();
                Map<Integer, Double> pushFront_pushBack_vec_memoryResult = new HashMap<>();
                Map<Integer, Double> popFront_popBack_ell_memoryResult = new HashMap<>();
                Map<Integer, Double> popFront_popBack_vec_memoryResult = new HashMap<>();

                Map<Integer, Double> pushBack_pushFront_ell_memoryResult = new HashMap<>();
                Map<Integer, Double> pushBack_pushFront_vec_memoryResult = new HashMap<>();
                Map<Integer, Double> popBack_popFront_ell_memoryResult = new HashMap<>();
                Map<Integer, Double> popBack_popFront_vec_memoryResult = new HashMap<>();

                // time taken results
                // push_back
                Map<Integer, Double> pushBack_ellResult = new HashMap<>();
                Map<Integer, Double> get_after_pushBack_ellResult = new HashMap<>();

                Map<Integer, Double> popBackAfterPushBack_ellResult = new HashMap<>();
                Map<Integer, Double> get_after_popBackAfterPushBack_ellResult = new HashMap<>();

                Map<Integer, Double> popFrontAfterPushBack_ellResult = new HashMap<>();
                Map<Integer, Double> get_after_popFrontAfterPushBack_ellResult = new HashMap<>();

                Map<Integer, Double> removeRandomIndicesAfterPushBack_ellResult = new HashMap<>();
                Map<Integer, Double> get_after_removeRandomIndicesAfterPushBack_ellResult = new HashMap<>();

                Map<Integer, Double> pushBack_vecResult = new HashMap<>();
                Map<Integer, Double> get_after_pushBack_vecResult = new HashMap<>();

                Map<Integer, Double> popBackAfterPushBack_vecResult = new HashMap<>();
                Map<Integer, Double> get_after_popBackAfterPushBack_vecResult = new HashMap<>();

                Map<Integer, Double> popFrontAfterPushBack_vecResult = new HashMap<>();
                Map<Integer, Double> get_after_popFrontAfterPushBack_vecResult = new HashMap<>();

                Map<Integer, Double> removeRandomIndicesAfterPushBack_vecResult = new HashMap<>();
                Map<Integer, Double> get_after_removeRandomIndicesAfterPushBack_vecResult = new HashMap<>();

                // push_front
                Map<Integer, Double> pushFront_ellResult = new HashMap<>();
                Map<Integer, Double> get_after_pushFront_ellResult = new HashMap<>();

                Map<Integer, Double> popFrontAfterPushFront_ellResult = new HashMap<>();
                Map<Integer, Double> get_after_popFrontAfterPushFront_ellResult = new HashMap<>();

                Map<Integer, Double> popBackAfterPushFront_ellResult = new HashMap<>();
                Map<Integer, Double> get_after_popBackAfterPushFront_ellResult = new HashMap<>();

                Map<Integer, Double> pushFront_vecResult = new HashMap<>();
                Map<Integer, Double> get_after_pushFront_vecResult = new HashMap<>();

                Map<Integer, Double> popFrontAfterPushFront_vecResult = new HashMap<>();
                Map<Integer, Double> get_after_popFrontAfterPushFront_vecResult = new HashMap<>();

                Map<Integer, Double> popBackAfterPushFront_vecResult = new HashMap<>();
                Map<Integer, Double> get_after_popBackAfterPushFront_vecResult = new HashMap<>();

                // insert random indices
                Map<Integer, Double> insertRandomIndices_ellResult = new HashMap<>();
                Map<Integer, Double> get_after_insertRandomIndices_ellResult = new HashMap<>();

                Map<Integer, Double> removeRandomIndices_ellResult = new HashMap<>();
                Map<Integer, Double> get_after_removeRandomIndices_ellResult = new HashMap<>();

                Map<Integer, Double> insertRandomIndices_vecResult = new HashMap<>();
                Map<Integer, Double> get_after_insertRandomIndices_vecResult = new HashMap<>();

                Map<Integer, Double> removeRandomIndices_vecResult = new HashMap<>();
                Map<Integer, Double> get_after_removeRandomIndices_vecResult = new HashMap<>();

                // push_front then push_back
                Map<Integer, Double> pushFront_pushBack_ellResult = new HashMap<>();
                Map<Integer, Double> get_after_pushFront_pushBack_ellResult = new HashMap<>();

                Map<Integer, Double> popFront_popBack_ellResult = new HashMap<>();
                Map<Integer, Double> get_after_popFront_popBack_ellResult = new HashMap<>();

                Map<Integer, Double> pushFront_pushBack_vecResult = new HashMap<>();
                Map<Integer, Double> get_after_pushFront_pushBack_vecResult = new HashMap<>();

                Map<Integer, Double> popFront_popBack_vecResult = new HashMap<>();
                Map<Integer, Double> get_after_popFront_popBack_vecResult = new HashMap<>();

                // push_back then push_front
                Map<Integer, Double> pushBack_pushFront_ellResult = new HashMap<>();
                Map<Integer, Double> get_after_pushBack_pushFront_ellResult = new HashMap<>();

                Map<Integer, Double> popBack_popFront_ellResult = new HashMap<>();
                Map<Integer, Double> get_after_popBack_popFront_ellResult = new HashMap<>();

                Map<Integer, Double> pushBack_pushFront_vecResult = new HashMap<>();
                Map<Integer, Double> get_after_pushBack_pushFront_vecResult = new HashMap<>();

                Map<Integer, Double> popBack_popFront_vecResult = new HashMap<>();
                Map<Integer, Double> get_after_popBack_popFront_vecResult = new HashMap<>();
                Random rand = new Random(99);

                for (int step = 1; step <= 1000000; step *= 10) {
                        for (int size = step; size < step * 10 && size <= 2000000; size += step) {

                                values = new int[size];
                                indexes = new int[size];
                                deleteIndexes = new int[size / 2];

                                for (int i = 0; i < size; i++) {
                                        values[i] = i + 1;
                                        indexes[i] = i;
                                }

                                boolean result = true;
                                AnchorList<Integer> ell;
                                ArrayList<Integer> vec;

                                System.out.println(
                                                "The test has started now with " + size + " insertions and "
                                                                + (size / 2) + " deletions:");

                                for (int input = 0; input < 8; input++) {
                                        switch (input) {
                                                case 0:
                                                        System.out.println(
                                                                        "\n1: test anchor list vs vector push_back then pop_back");
                                                        ell = new AnchorList<>();
                                                        vec = new ArrayList<>();
                                                        insertToAnchorList(ell, size, pushBack_ellResult,
                                                                        pushBack_ell_memoryResult);
                                                        insertToVector(vec, size, pushBack_vecResult,
                                                                        pushBack_vec_memoryResult);
                                                        result = valueCheck(ell, vec, size,
                                                                        get_after_pushBack_ellResult,
                                                                        get_after_pushBack_vecResult);
                                                        if (!result) {
                                                                break;
                                                        }

                                                        for (int i = 0; i < size / 2; i++) {
                                                                deleteIndexes[i] = size - (i + 1);
                                                        }
                                                        removeFromAnchorList(ell, size / 2,
                                                                        popBackAfterPushBack_ellResult,
                                                                        popBackAfterPushBack_ell_memoryResult);
                                                        removeFromVector(vec, size / 2, popBackAfterPushBack_vecResult,
                                                                        popBackAfterPushBack_vec_memoryResult);

                                                        result = valueCheck(ell, vec, size / 2,
                                                                        get_after_popBackAfterPushBack_ellResult,
                                                                        get_after_popBackAfterPushBack_vecResult);
                                                        break;
                                                case 1:
                                                        System.out.println(
                                                                        "\n2: test anchor list vs vector push_back then pop_front");
                                                        ell = new AnchorList<>();
                                                        vec = new ArrayList<>();
                                                        insertToAnchorList(ell, size, null, null);
                                                        insertToVector(vec, size, null, null);

                                                        for (int i = 0; i < size / 2; i++) {
                                                                deleteIndexes[i] = 0;
                                                        }
                                                        removeFromAnchorList(ell, size / 2,
                                                                        popFrontAfterPushBack_ellResult,
                                                                        popFrontAfterPushBack_ell_memoryResult);
                                                        removeFromVector(vec, size / 2, popFrontAfterPushBack_vecResult,
                                                                        popFrontAfterPushBack_vec_memoryResult);
                                                        result = valueCheck(ell, vec, size / 2,
                                                                        get_after_popFrontAfterPushBack_ellResult,
                                                                        get_after_popFrontAfterPushBack_vecResult);
                                                        break;
                                                case 2:
                                                        System.out.println(
                                                                        "\n3: test anchor list vs vector push_back then random indices remove");
                                                        ell = new AnchorList<>();
                                                        vec = new ArrayList<>();
                                                        insertToAnchorList(ell, size, null, null);
                                                        insertToVector(vec, size, null, null);

                                                        for (int i = 0; i < size / 2; i++) {
                                                                deleteIndexes[i] = rand.nextInt(size - (i + 1));
                                                        }
                                                        removeFromAnchorList(ell, size / 2,
                                                                        removeRandomIndicesAfterPushBack_ellResult,
                                                                        removeRandomIndicesAfterPushBack_ell_memoryResult);
                                                        removeFromVector(vec, size / 2,
                                                                        removeRandomIndicesAfterPushBack_vecResult,
                                                                        removeRandomIndicesAfterPushBack_vec_memoryResult);
                                                        result = valueCheck(ell, vec, size / 2,
                                                                        get_after_removeRandomIndicesAfterPushBack_ellResult,
                                                                        get_after_removeRandomIndicesAfterPushBack_vecResult);
                                                        break;
                                                case 3:
                                                        System.out.println(
                                                                        "\n4: test anchor list vs vector push_front then pop_front");
                                                        ell = new AnchorList<>();
                                                        vec = new ArrayList<>();
                                                        for (int i = 0; i < size; i++) {
                                                                indexes[i] = 0;
                                                        }
                                                        insertToAnchorList(ell, size, pushFront_ellResult,
                                                                        pushFront_ell_memoryResult);
                                                        insertToVector(vec, size, pushFront_vecResult,
                                                                        pushFront_vec_memoryResult);
                                                        result = valueCheck(ell, vec, size,
                                                                        get_after_pushFront_ellResult,
                                                                        get_after_pushFront_vecResult);
                                                        if (!result) {
                                                                break;
                                                        }

                                                        for (int i = 0; i < size / 2; i++) {
                                                                deleteIndexes[i] = 0;
                                                        }
                                                        removeFromAnchorList(ell, size / 2,
                                                                        popFrontAfterPushFront_ellResult,
                                                                        popFrontAfterPushFront_ell_memoryResult);
                                                        removeFromVector(vec, size / 2,
                                                                        popFrontAfterPushFront_vecResult,
                                                                        popFrontAfterPushFront_vec_memoryResult);
                                                        result = valueCheck(ell, vec, size / 2,
                                                                        get_after_popFrontAfterPushFront_ellResult,
                                                                        get_after_popFrontAfterPushFront_vecResult);
                                                        break;

                                                case 4:
                                                        System.out.println(
                                                                        "\n5: test anchor list vs vector push_front then pop_back");
                                                        ell = new AnchorList<>();
                                                        vec = new ArrayList<>();
                                                        insertToAnchorList(ell, size, null, null);
                                                        insertToVector(vec, size, null, null);

                                                        for (int i = 0; i < size / 2; i++) {
                                                                deleteIndexes[i] = size - (i + 1);
                                                        }
                                                        removeFromAnchorList(ell, size / 2,
                                                                        popBackAfterPushFront_ellResult,
                                                                        popBackAfterPushFront_ell_memoryResult);
                                                        removeFromVector(vec, size / 2, popBackAfterPushFront_vecResult,
                                                                        popBackAfterPushFront_vec_memoryResult);
                                                        result = valueCheck(ell, vec, size / 2,
                                                                        get_after_popBackAfterPushFront_ellResult,
                                                                        get_after_popBackAfterPushFront_vecResult);
                                                        break;

                                                case 5:
                                                        System.out.println(
                                                                        "\n6: test anchor list vs vector random indices insert then random indices remove");
                                                        ell = new AnchorList<>();
                                                        vec = new ArrayList<>();

                                                        for (int i = 0; i < size; i++) {
                                                                indexes[i] = rand.nextInt(i + 1);
                                                        }
                                                        insertToAnchorList(ell, size,
                                                                        insertRandomIndices_ellResult,
                                                                        insertRandomIndices_ell_memoryResult);
                                                        insertToVector(vec, size, insertRandomIndices_vecResult,
                                                                        insertRandomIndices_vec_memoryResult);
                                                        result = valueCheck(ell, vec, size,
                                                                        get_after_insertRandomIndices_ellResult,
                                                                        get_after_insertRandomIndices_vecResult);
                                                        if (!result) {
                                                                break;
                                                        }
                                                        for (int i = 0; i < size / 2; i++) {
                                                                deleteIndexes[i] = rand.nextInt(size - (i + 1));
                                                        }
                                                        removeFromAnchorList(ell, size / 2,
                                                                        removeRandomIndices_ellResult,
                                                                        removeRandomIndices_ell_memoryResult);
                                                        removeFromVector(vec, size / 2, removeRandomIndices_vecResult,
                                                                        removeRandomIndices_vec_memoryResult);
                                                        result = valueCheck(ell, vec, size / 2,
                                                                        get_after_removeRandomIndices_ellResult,
                                                                        get_after_removeRandomIndices_vecResult);
                                                        break;

                                                case 6:
                                                        System.out.println(
                                                                        "\n7: test anchor list vs vector half push_front then push_back the other half, then half pop_front then pop_back the other half");
                                                        ell = new AnchorList<>();
                                                        vec = new ArrayList<>();
                                                        for (int i = 0; i < size / 2; i++) {
                                                                indexes[i] = 0;
                                                                indexes[i + size / 2] = i + size / 2;
                                                        }
                                                        insertToAnchorList(ell, size,
                                                                        pushFront_pushBack_ellResult,
                                                                        pushFront_pushBack_ell_memoryResult);
                                                        insertToVector(vec, size, pushFront_pushBack_vecResult,
                                                                        pushFront_pushBack_vec_memoryResult);

                                                        result = valueCheck(ell, vec, size,
                                                                        get_after_pushFront_pushBack_ellResult,
                                                                        get_after_pushFront_pushBack_vecResult);
                                                        if (!result) {
                                                                break;
                                                        }
                                                        for (int i = 0; i < size / 4; i++) {
                                                                deleteIndexes[i] = 0;
                                                                deleteIndexes[i + size / 4] = size - (i + size / 4 + 1);
                                                        }
                                                        removeFromAnchorList(ell, size / 2,
                                                                        popFront_popBack_ellResult,
                                                                        popFront_popBack_ell_memoryResult);
                                                        removeFromVector(vec, size / 2, popFront_popBack_vecResult,
                                                                        popFront_popBack_vec_memoryResult);

                                                        result = valueCheck(ell, vec, size / 2,
                                                                        get_after_popFront_popBack_ellResult,
                                                                        get_after_popFront_popBack_vecResult);
                                                        break;

                                                case 7:
                                                        System.out.println(
                                                                        "\n8: test anchor list vs vector half push_back then push_front the other half, then half pop_back then pop_front the other half");
                                                        ell = new AnchorList<>();
                                                        vec = new ArrayList<>();
                                                        for (int i = 0; i < size / 2; i++) {
                                                                indexes[i] = i;
                                                                indexes[i + size / 2] = 0;
                                                        }
                                                        insertToAnchorList(ell, size,
                                                                        pushBack_pushFront_ellResult,
                                                                        pushBack_pushFront_ell_memoryResult);
                                                        insertToVector(vec, size, pushBack_pushFront_vecResult,
                                                                        pushBack_pushFront_vec_memoryResult);
                                                        result = valueCheck(ell, vec, size,
                                                                        get_after_pushBack_pushFront_ellResult,
                                                                        get_after_pushBack_pushFront_vecResult);
                                                        if (!result) {
                                                                break;
                                                        }
                                                        for (int i = 0; i < size / 4; i++) {
                                                                deleteIndexes[i] = size - (i + 1);
                                                                deleteIndexes[i + size / 4] = 0;
                                                        }
                                                        removeFromAnchorList(ell, size / 2,
                                                                        popBack_popFront_ellResult,
                                                                        popBack_popFront_ell_memoryResult);
                                                        removeFromVector(vec, size / 2, popBack_popFront_vecResult,
                                                                        popBack_popFront_vec_memoryResult);

                                                        result = valueCheck(ell, vec, size / 2,
                                                                        get_after_popBack_popFront_ellResult,
                                                                        get_after_popBack_popFront_vecResult);
                                                        break;
                                        }

                                        if (result) {
                                                System.out.println("** That was correct!");
                                        } else {
                                                System.out.println("** Doesn't match.");
                                                System.out.println("Your code did not pass the tests.");
                                                return;
                                        }
                                }
                        }
                }
                // Save all memory results
                saveMapToCSV(pushBack_ell_memoryResult, "memoryResults/pushBack_ell_memory_results.csv");
                saveMapToCSV(pushBack_vec_memoryResult, "memoryResults/pushBack_vec_memory_results.csv");

                saveMapToCSV(popBackAfterPushBack_ell_memoryResult,
                                "memoryResults/popBackAfterPushBack_ell_memory_results.csv");
                saveMapToCSV(popBackAfterPushBack_vec_memoryResult,
                                "memoryResults/popBackAfterPushBack_vec_memory_results.csv");

                saveMapToCSV(popFrontAfterPushBack_ell_memoryResult,
                                "memoryResults/popFrontAfterPushBack_ell_memory_results.csv");
                saveMapToCSV(popFrontAfterPushBack_vec_memoryResult,
                                "memoryResults/popFrontAfterPushBack_vec_memory_results.csv");

                saveMapToCSV(removeRandomIndicesAfterPushBack_ell_memoryResult,
                                "memoryResults/removeRandomIndicesAfterPushBack_ell_memory_results.csv");
                saveMapToCSV(removeRandomIndicesAfterPushBack_vec_memoryResult,
                                "memoryResults/removeRandomIndicesAfterPushBack_vec_memory_results.csv");

                saveMapToCSV(pushFront_ell_memoryResult, "memoryResults/pushFront_ell_memory_results.csv");
                saveMapToCSV(pushFront_vec_memoryResult, "memoryResults/pushFront_vec_memory_results.csv");

                saveMapToCSV(popFrontAfterPushFront_ell_memoryResult,
                                "memoryResults/popFrontAfterPushFront_ell_memory_results.csv");
                saveMapToCSV(popFrontAfterPushFront_vec_memoryResult,
                                "memoryResults/popFrontAfterPushFront_vec_memory_results.csv");

                saveMapToCSV(popBackAfterPushFront_ell_memoryResult,
                                "memoryResults/popBackAfterPushFront_ell_memory_results.csv");
                saveMapToCSV(popBackAfterPushFront_vec_memoryResult,
                                "memoryResults/popBackAfterPushFront_vec_memory_results.csv");

                saveMapToCSV(insertRandomIndices_ell_memoryResult,
                                "memoryResults/insertRandomIndices_ell_memory_results.csv");
                saveMapToCSV(insertRandomIndices_vec_memoryResult,
                                "memoryResults/insertRandomIndices_vec_memory_results.csv");

                saveMapToCSV(removeRandomIndices_ell_memoryResult,
                                "memoryResults/removeRandomIndices_ell_memory_results.csv");
                saveMapToCSV(removeRandomIndices_vec_memoryResult,
                                "memoryResults/removeRandomIndices_vec_memory_results.csv");

                saveMapToCSV(pushFront_pushBack_ell_memoryResult,
                                "memoryResults/pushFront_pushBack_ell_memory_results.csv");
                saveMapToCSV(pushFront_pushBack_vec_memoryResult,
                                "memoryResults/pushFront_pushBack_vec_memory_results.csv");

                saveMapToCSV(popFront_popBack_ell_memoryResult,
                                "memoryResults/popFront_popBack_ell_memory_results.csv");
                saveMapToCSV(popFront_popBack_vec_memoryResult,
                                "memoryResults/popFront_popBack_vec_memory_results.csv");

                saveMapToCSV(pushBack_pushFront_ell_memoryResult,
                                "memoryResults/pushBack_pushFront_ell_memory_results.csv");
                saveMapToCSV(pushBack_pushFront_vec_memoryResult,
                                "memoryResults/pushBack_pushFront_vec_memory_results.csv");

                saveMapToCSV(popBack_popFront_ell_memoryResult,
                                "memoryResults/popBack_popFront_ell_memory_results.csv");
                saveMapToCSV(popBack_popFront_vec_memoryResult,
                                "memoryResults/popBack_popFront_vec_memory_results.csv");

                // Save all timing results
                saveMapToCSV(pushBack_ellResult, "timeTakenResults/pushBack_ell_results.csv");
                saveMapToCSV(get_after_pushBack_ellResult, "timeTakenResults/get_after_pushBack_ell_results.csv");

                saveMapToCSV(popBackAfterPushBack_ellResult, "timeTakenResults/popBackAfterPushBack_ell_results.csv");
                saveMapToCSV(get_after_popBackAfterPushBack_ellResult,
                                "timeTakenResults/get_after_popBackAfterPushBack_ell_results.csv");

                saveMapToCSV(popFrontAfterPushBack_ellResult, "timeTakenResults/popFrontAfterPushBack_ell_results.csv");
                saveMapToCSV(get_after_popFrontAfterPushBack_ellResult,
                                "timeTakenResults/get_after_popFrontAfterPushBack_ell_results.csv");

                saveMapToCSV(removeRandomIndicesAfterPushBack_ellResult,
                                "timeTakenResults/removeRandomIndicesAfterPushBack_ell_results.csv");
                saveMapToCSV(get_after_removeRandomIndicesAfterPushBack_ellResult,
                                "timeTakenResults/get_after_removeRandomIndicesAfterPushBack_ell_results.csv");

                saveMapToCSV(pushBack_vecResult, "timeTakenResults/pushBack_vec_results.csv");
                saveMapToCSV(get_after_pushBack_vecResult, "timeTakenResults/get_after_pushBack_vec_results.csv");

                saveMapToCSV(popBackAfterPushBack_vecResult, "timeTakenResults/popBackAfterPushBack_vec_results.csv");
                saveMapToCSV(get_after_popBackAfterPushBack_vecResult,
                                "timeTakenResults/get_after_popBackAfterPushBack_vec_results.csv");

                saveMapToCSV(popFrontAfterPushBack_vecResult, "timeTakenResults/popFrontAfterPushBack_vec_results.csv");
                saveMapToCSV(get_after_popFrontAfterPushBack_vecResult,
                                "timeTakenResults/get_after_popFrontAfterPushBack_vec_results.csv");

                saveMapToCSV(removeRandomIndicesAfterPushBack_vecResult,
                                "timeTakenResults/removeRandomIndicesAfterPushBack_vec_results.csv");
                saveMapToCSV(get_after_removeRandomIndicesAfterPushBack_vecResult,
                                "timeTakenResults/get_after_removeRandomIndicesAfterPushBack_vec_results.csv");

                saveMapToCSV(pushFront_ellResult, "timeTakenResults/pushFront_ell_results.csv");
                saveMapToCSV(get_after_pushFront_ellResult, "timeTakenResults/get_after_pushFront_ell_results.csv");

                saveMapToCSV(popFrontAfterPushFront_ellResult,
                                "timeTakenResults/popFrontAfterPushFront_ell_results.csv");
                saveMapToCSV(get_after_popFrontAfterPushFront_ellResult,
                                "timeTakenResults/get_after_popFrontAfterPushFront_ell_results.csv");

                saveMapToCSV(popBackAfterPushFront_ellResult, "timeTakenResults/popBackAfterPushFront_ell_results.csv");
                saveMapToCSV(get_after_popBackAfterPushFront_ellResult,
                                "timeTakenResults/get_after_popBackAfterPushFront_ell_results.csv");

                saveMapToCSV(pushFront_vecResult, "timeTakenResults/pushFront_vec_results.csv");
                saveMapToCSV(get_after_pushFront_vecResult, "timeTakenResults/get_after_pushFront_vec_results.csv");

                saveMapToCSV(popFrontAfterPushFront_vecResult,
                                "timeTakenResults/popFrontAfterPushFront_vec_results.csv");
                saveMapToCSV(get_after_popFrontAfterPushFront_vecResult,
                                "timeTakenResults/get_after_popFrontAfterPushFront_vec_results.csv");

                saveMapToCSV(popBackAfterPushFront_vecResult, "timeTakenResults/popBackAfterPushFront_vec_results.csv");
                saveMapToCSV(get_after_popBackAfterPushFront_vecResult,
                                "timeTakenResults/get_after_popBackAfterPushFront_vec_results.csv");

                saveMapToCSV(insertRandomIndices_ellResult, "timeTakenResults/insertRandomIndices_ell_results.csv");
                saveMapToCSV(get_after_insertRandomIndices_ellResult,
                                "timeTakenResults/get_after_insertRandomIndices_ell_results.csv");

                saveMapToCSV(removeRandomIndices_ellResult, "timeTakenResults/removeRandomIndices_ell_results.csv");
                saveMapToCSV(get_after_removeRandomIndices_ellResult,
                                "timeTakenResults/get_after_removeRandomIndices_ell_results.csv");

                saveMapToCSV(insertRandomIndices_vecResult, "timeTakenResults/insertRandomIndices_vec_results.csv");
                saveMapToCSV(get_after_insertRandomIndices_vecResult,
                                "timeTakenResults/get_after_insertRandomIndices_vec_results.csv");

                saveMapToCSV(removeRandomIndices_vecResult, "timeTakenResults/removeRandomIndices_vec_results.csv");
                saveMapToCSV(get_after_removeRandomIndices_vecResult,
                                "timeTakenResults/get_after_removeRandomIndices_vec_results.csv");

                saveMapToCSV(pushFront_pushBack_ellResult, "timeTakenResults/pushFront_pushBack_ell_results.csv");
                saveMapToCSV(get_after_pushFront_pushBack_ellResult,
                                "timeTakenResults/get_after_pushFront_pushBack_ell_results.csv");

                saveMapToCSV(popFront_popBack_ellResult, "timeTakenResults/popFront_popBack_ell_results.csv");
                saveMapToCSV(get_after_popFront_popBack_ellResult,
                                "timeTakenResults/get_after_popFront_popBack_ell_results.csv");

                saveMapToCSV(pushFront_pushBack_vecResult, "timeTakenResults/pushFront_pushBack_vec_results.csv");
                saveMapToCSV(get_after_pushFront_pushBack_vecResult,
                                "timeTakenResults/get_after_pushFront_pushBack_vec_results.csv");

                saveMapToCSV(popFront_popBack_vecResult, "timeTakenResults/popFront_popBack_vec_results.csv");
                saveMapToCSV(get_after_popFront_popBack_vecResult,
                                "timeTakenResults/get_after_popFront_popBack_vec_results.csv");

                saveMapToCSV(pushBack_pushFront_ellResult, "timeTakenResults/pushBack_pushFront_ell_results.csv");
                saveMapToCSV(get_after_pushBack_pushFront_ellResult,
                                "timeTakenResults/get_after_pushBack_pushFront_ell_results.csv");

                saveMapToCSV(popBack_popFront_ellResult, "timeTakenResults/popBack_popFront_ell_results.csv");
                saveMapToCSV(get_after_popBack_popFront_ellResult,
                                "timeTakenResults/get_after_popBack_popFront_ell_results.csv");

                saveMapToCSV(pushBack_pushFront_vecResult, "timeTakenResults/pushBack_pushFront_vec_results.csv");
                saveMapToCSV(get_after_pushBack_pushFront_vecResult,
                                "timeTakenResults/get_after_pushBack_pushFront_vec_results.csv");

                saveMapToCSV(popBack_popFront_vecResult, "timeTakenResults/popBack_popFront_vec_results.csv");
                saveMapToCSV(get_after_popBack_popFront_vecResult,
                                "timeTakenResults/get_after_popBack_popFront_vec_results.csv");

                System.out.println("Your code passed all the tests.");
        }
}