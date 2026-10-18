import java.util.*;

public class AnchorList_vs_arraylist_half_remove {
        static int[] values;
        static int[] indexes;
        static int[] deleteIndexes;
        static long startTime, endTime;

        private static void insertToAnchorList(AnchorList<Integer> anchor, int size,
                        Map<Integer, Double> timeTakenMap)
                        throws IllegalAccessException {

                long startTime = System.nanoTime();
                for (int i = 0; i < size; i++) {
                        // System.out.println("index: " + indexes[i] + "\t value:" + values[i]);
                        anchor.insert(indexes[i], values[i]);
                        // anchor.drawTree();
                        // anchor.iterateInOrder();
                        // System.out.println("===============");
                }
                long endTime = System.nanoTime();

                Double timeTaken = ((endTime - startTime) / 1_000_000_000.0); // seconds
                if (timeTakenMap != null) {
                        timeTakenMap.put(size, timeTaken);
                }
                // System.out.println("insert time taken anchor list: " + timeTaken);
        }

        private static void insertToArrayList(ArrayList<Integer> arrayList, int size,
                        Map<Integer, Double> timeTakenMap) {

                long startTime = System.nanoTime();
                for (int i = 0; i < size; i++) {
                        arrayList.add(indexes[i], values[i]);
                }
                long endTime = System.nanoTime();

                Double timeTaken = ((endTime - startTime) / 1_000_000_000.0); // seconds
                if (timeTakenMap != null) {
                        timeTakenMap.put(size, timeTaken);
                }
                // System.out.println("insert time taken arrayList: " + timeTaken);
        }

        private static boolean valueCheck(
                        AnchorList<Integer> anchor,
                        ArrayList<Integer> arrayList,
                        int size,
                        Map<Integer, Double> anchorTimeTakenMap,
                        Map<Integer, Double> arrayListTimeTakenMap) throws IllegalAccessException {
                boolean result = true;
                int anchorValue, arrayListValue;

                // Check for value equality
                for (int i = 0; i < size; i++) {
                        if (!anchor.get(i).equals(arrayList.get(i))) {
                                // System.out.println(anchor.get(i) + " " + arrayList.get(i));
                                result = false;
                                break;
                        }
                }

                // Measure AnchorList get time
                long startTime = System.nanoTime();
                for (int i = 0; i < size; i++) {
                        anchorValue = anchor.get(i);
                }
                long endTime = System.nanoTime();
                Double anchorTimeTaken = ((endTime - startTime) / 1_000_000_000.0);

                // Measure ArrayList get time
                startTime = System.nanoTime();
                for (int i = 0; i < size; i++) {
                        arrayListValue = arrayList.get(i);
                }
                endTime = System.nanoTime();
                Double arrayListTimeTaken = ((endTime - startTime) / 1_000_000_000.0);

                // Store timings in maps if provided
                if (anchorTimeTakenMap != null) {
                        anchorTimeTakenMap.put(size, anchorTimeTaken);
                }
                if (arrayListTimeTakenMap != null) {
                        arrayListTimeTakenMap.put(size, arrayListTimeTaken);
                }

                // Optionally print timings
                // System.out.println("anchor get time taken: " + anchorTimeTaken);
                // System.out.println("arrayList get time taken: " + arrayListTimeTaken);

                return result;
        }

        private static void removeFromAnchorList(AnchorList<Integer> anchor, int deleteIndexesSize,
                        Map<Integer, Double> timeTakenMap)
                        throws IllegalAccessException {
                long startTime = System.nanoTime();
                for (int i = 0; i < deleteIndexesSize; i++) {
                        anchor.remove(deleteIndexes[i]);
                }
                long endTime = System.nanoTime();

                Double timeTaken = ((endTime - startTime) / 1_000_000_000.0); // seconds
                if (timeTakenMap != null) {
                        timeTakenMap.put(deleteIndexesSize, timeTaken);
                }
                // System.out.println("remove time taken anchor list: " + timeTaken);
        }

        private static void removeFromArrayList(ArrayList<Integer> arrayList, int deleteIndexesSize,
                        Map<Integer, Double> timeTakenMap) {

                long startTime = System.nanoTime();
                for (int i = 0; i < deleteIndexesSize; i++) {
                        arrayList.remove(deleteIndexes[i]);
                }
                long endTime = System.nanoTime();

                Double timeTaken = ((endTime - startTime) / 1_000_000_000.0); // seconds
                if (timeTakenMap != null) {
                        timeTakenMap.put(deleteIndexesSize, timeTaken);
                }
                // System.out.println("remove time taken arrayList: " + timeTaken);
        }

        public static void saveMapToCSV(Map<Integer, Double> data, String filename) {
                java.nio.file.Path fullPath = java.nio.file.Paths.get(
                                System.getProperty("user.dir"), filename);
                try (java.io.PrintWriter file = new java.io.PrintWriter(fullPath.toFile())) {
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

                // time taken results
                // push_back
                Map<Integer, Double> pushBack_anchorResult = new HashMap<>();
                Map<Integer, Double> get_after_pushBack_anchorResult = new HashMap<>();

                Map<Integer, Double> popBackAfterPushBack_anchorResult = new HashMap<>();
                Map<Integer, Double> get_after_popBackAfterPushBack_anchorResult = new HashMap<>();

                Map<Integer, Double> popFrontAfterPushBack_anchorResult = new HashMap<>();
                Map<Integer, Double> get_after_popFrontAfterPushBack_anchorResult = new HashMap<>();

                Map<Integer, Double> removeRandomIndicesAfterPushBack_anchorResult = new HashMap<>();
                Map<Integer, Double> get_after_removeRandomIndicesAfterPushBack_anchorResult = new HashMap<>();

                Map<Integer, Double> pushBack_arrayListResult = new HashMap<>();
                Map<Integer, Double> get_after_pushBack_arrayListResult = new HashMap<>();

                Map<Integer, Double> popBackAfterPushBack_arrayListResult = new HashMap<>();
                Map<Integer, Double> get_after_popBackAfterPushBack_arrayListResult = new HashMap<>();

                Map<Integer, Double> popFrontAfterPushBack_arrayListResult = new HashMap<>();
                Map<Integer, Double> get_after_popFrontAfterPushBack_arrayListResult = new HashMap<>();

                Map<Integer, Double> removeRandomIndicesAfterPushBack_arrayListResult = new HashMap<>();
                Map<Integer, Double> get_after_removeRandomIndicesAfterPushBack_arrayListResult = new HashMap<>();

                // push_front
                Map<Integer, Double> pushFront_anchorResult = new HashMap<>();
                Map<Integer, Double> get_after_pushFront_anchorResult = new HashMap<>();

                Map<Integer, Double> popFrontAfterPushFront_anchorResult = new HashMap<>();
                Map<Integer, Double> get_after_popFrontAfterPushFront_anchorResult = new HashMap<>();

                Map<Integer, Double> popBackAfterPushFront_anchorResult = new HashMap<>();
                Map<Integer, Double> get_after_popBackAfterPushFront_anchorResult = new HashMap<>();

                Map<Integer, Double> pushFront_arrayListResult = new HashMap<>();
                Map<Integer, Double> get_after_pushFront_arrayListResult = new HashMap<>();

                Map<Integer, Double> popFrontAfterPushFront_arrayListResult = new HashMap<>();
                Map<Integer, Double> get_after_popFrontAfterPushFront_arrayListResult = new HashMap<>();

                Map<Integer, Double> popBackAfterPushFront_arrayListResult = new HashMap<>();
                Map<Integer, Double> get_after_popBackAfterPushFront_arrayListResult = new HashMap<>();

                // insert random indices
                Map<Integer, Double> insertRandomIndices_anchorResult = new HashMap<>();
                Map<Integer, Double> get_after_insertRandomIndices_anchorResult = new HashMap<>();

                Map<Integer, Double> removeRandomIndices_anchorResult = new HashMap<>();
                Map<Integer, Double> get_after_removeRandomIndices_anchorResult = new HashMap<>();

                Map<Integer, Double> insertRandomIndices_arrayListResult = new HashMap<>();
                Map<Integer, Double> get_after_insertRandomIndices_arrayListResult = new HashMap<>();

                Map<Integer, Double> removeRandomIndices_arrayListResult = new HashMap<>();
                Map<Integer, Double> get_after_removeRandomIndices_arrayListResult = new HashMap<>();

                // push_front then push_back
                Map<Integer, Double> pushFront_pushBack_anchorResult = new HashMap<>();
                Map<Integer, Double> get_after_pushFront_pushBack_anchorResult = new HashMap<>();

                Map<Integer, Double> popFront_popBack_anchorResult = new HashMap<>();
                Map<Integer, Double> get_after_popFront_popBack_anchorResult = new HashMap<>();

                Map<Integer, Double> pushFront_pushBack_arrayListResult = new HashMap<>();
                Map<Integer, Double> get_after_pushFront_pushBack_arrayListResult = new HashMap<>();

                Map<Integer, Double> popFront_popBack_arrayListResult = new HashMap<>();
                Map<Integer, Double> get_after_popFront_popBack_arrayListResult = new HashMap<>();

                // push_back then push_front
                Map<Integer, Double> pushBack_pushFront_anchorResult = new HashMap<>();
                Map<Integer, Double> get_after_pushBack_pushFront_anchorResult = new HashMap<>();

                Map<Integer, Double> popBack_popFront_anchorResult = new HashMap<>();
                Map<Integer, Double> get_after_popBack_popFront_anchorResult = new HashMap<>();

                Map<Integer, Double> pushBack_pushFront_arrayListResult = new HashMap<>();
                Map<Integer, Double> get_after_pushBack_pushFront_arrayListResult = new HashMap<>();

                Map<Integer, Double> popBack_popFront_arrayListResult = new HashMap<>();
                Map<Integer, Double> get_after_popBack_popFront_arrayListResult = new HashMap<>();
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
                                AnchorList<Integer> anchor;
                                ArrayList<Integer> arrayList;

                                System.out.println(
                                                "The test has started now with " + size + " insertions and "
                                                                + (size / 2) + " deletions:");

                                for (int input = 0; input < 8; input++) {
                                        switch (input) {
                                                case 0:
                                                        /*
                                                         * System.out.println(
                                                         * "\n1: test anchor list vs arrayList push_back then pop_back"
                                                         * );
                                                         */
                                                        anchor = new AnchorList<>();
                                                        arrayList = new ArrayList<>();
                                                        insertToAnchorList(anchor, size, pushBack_anchorResult);
                                                        insertToArrayList(arrayList, size, pushBack_arrayListResult);
                                                        result = valueCheck(anchor, arrayList, size,
                                                                        get_after_pushBack_anchorResult,
                                                                        get_after_pushBack_arrayListResult);
                                                        if (!result) {
                                                                break;
                                                        }

                                                        for (int i = 0; i < size / 2; i++) {
                                                                deleteIndexes[i] = size - (i + 1);
                                                        }
                                                        removeFromAnchorList(anchor, size / 2,
                                                                        popBackAfterPushBack_anchorResult);
                                                        removeFromArrayList(arrayList, size / 2,
                                                                        popBackAfterPushBack_arrayListResult);

                                                        result = valueCheck(anchor, arrayList, size / 2,
                                                                        get_after_popBackAfterPushBack_anchorResult,
                                                                        get_after_popBackAfterPushBack_arrayListResult);
                                                        break;
                                                case 1:
                                                        /*
                                                         * System.out.println(
                                                         * "\n2: test anchor list vs arrayList push_back then pop_front"
                                                         * );
                                                         */
                                                        anchor = new AnchorList<>();
                                                        arrayList = new ArrayList<>();
                                                        insertToAnchorList(anchor, size, null);
                                                        insertToArrayList(arrayList, size, null);

                                                        for (int i = 0; i < size / 2; i++) {
                                                                deleteIndexes[i] = 0;
                                                        }
                                                        removeFromAnchorList(anchor, size / 2,
                                                                        popFrontAfterPushBack_anchorResult);
                                                        removeFromArrayList(arrayList, size / 2,
                                                                        popFrontAfterPushBack_arrayListResult);
                                                        result = valueCheck(anchor, arrayList, size / 2,
                                                                        get_after_popFrontAfterPushBack_anchorResult,
                                                                        get_after_popFrontAfterPushBack_arrayListResult);
                                                        break;
                                                case 2:
                                                        /*
                                                         * System.out.println(
                                                         * "\n3: test anchor list vs arrayList push_back then random indices remove"
                                                         * );
                                                         */
                                                        anchor = new AnchorList<>();
                                                        arrayList = new ArrayList<>();
                                                        insertToAnchorList(anchor, size, null);
                                                        insertToArrayList(arrayList, size, null);

                                                        for (int i = 0; i < size / 2; i++) {
                                                                deleteIndexes[i] = rand.nextInt(size - (i + 1));
                                                        }
                                                        removeFromAnchorList(anchor, size / 2,
                                                                        removeRandomIndicesAfterPushBack_anchorResult);
                                                        removeFromArrayList(arrayList, size / 2,
                                                                        removeRandomIndicesAfterPushBack_arrayListResult);
                                                        result = valueCheck(anchor, arrayList, size / 2,
                                                                        get_after_removeRandomIndicesAfterPushBack_anchorResult,
                                                                        get_after_removeRandomIndicesAfterPushBack_arrayListResult);
                                                        break;
                                                case 3:
                                                        /*
                                                         * System.out.println(
                                                         * "\n4: test anchor list vs arrayList push_front then pop_front"
                                                         * );
                                                         */
                                                        anchor = new AnchorList<>();
                                                        arrayList = new ArrayList<>();
                                                        for (int i = 0; i < size; i++) {
                                                                indexes[i] = 0;
                                                        }
                                                        insertToAnchorList(anchor, size, pushFront_anchorResult);
                                                        insertToArrayList(arrayList, size, pushFront_arrayListResult);
                                                        result = valueCheck(anchor, arrayList, size,
                                                                        get_after_pushFront_anchorResult,
                                                                        get_after_pushFront_arrayListResult);
                                                        if (!result) {
                                                                break;
                                                        }

                                                        for (int i = 0; i < size / 2; i++) {
                                                                deleteIndexes[i] = 0;
                                                        }
                                                        removeFromAnchorList(anchor, size / 2,
                                                                        popFrontAfterPushFront_anchorResult);
                                                        removeFromArrayList(arrayList, size / 2,
                                                                        popFrontAfterPushFront_arrayListResult);
                                                        result = valueCheck(anchor, arrayList, size / 2,
                                                                        get_after_popFrontAfterPushFront_anchorResult,
                                                                        get_after_popFrontAfterPushFront_arrayListResult);
                                                        break;

                                                case 4:
                                                        /*
                                                         * System.out.println(
                                                         * "\n5: test anchor list vs arrayList push_front then pop_back"
                                                         * );
                                                         */
                                                        anchor = new AnchorList<>();
                                                        arrayList = new ArrayList<>();
                                                        insertToAnchorList(anchor, size, null);
                                                        insertToArrayList(arrayList, size, null);

                                                        for (int i = 0; i < size / 2; i++) {
                                                                deleteIndexes[i] = size - (i + 1);
                                                        }
                                                        removeFromAnchorList(anchor, size / 2,
                                                                        popBackAfterPushFront_anchorResult);
                                                        removeFromArrayList(arrayList, size / 2,
                                                                        popBackAfterPushFront_arrayListResult);
                                                        result = valueCheck(anchor, arrayList, size / 2,
                                                                        get_after_popBackAfterPushFront_anchorResult,
                                                                        get_after_popBackAfterPushFront_arrayListResult);
                                                        break;

                                                case 5:
                                                        /*
                                                         * System.out.println(
                                                         * "\n6: test anchor list vs arrayList random indices insert then random indices remove"
                                                         * );
                                                         */
                                                        anchor = new AnchorList<>();
                                                        arrayList = new ArrayList<>();

                                                        for (int i = 0; i < size; i++) {
                                                                indexes[i] = rand.nextInt(i + 1);
                                                        }
                                                        insertToAnchorList(anchor, size,
                                                                        insertRandomIndices_anchorResult);
                                                        insertToArrayList(arrayList, size,
                                                                        insertRandomIndices_arrayListResult);
                                                        result = valueCheck(anchor, arrayList, size,
                                                                        get_after_insertRandomIndices_anchorResult,
                                                                        get_after_insertRandomIndices_arrayListResult);
                                                        if (!result) {
                                                                break;
                                                        }
                                                        for (int i = 0; i < size / 2; i++) {
                                                                deleteIndexes[i] = rand.nextInt(size - (i + 1));
                                                        }
                                                        removeFromAnchorList(anchor, size / 2,
                                                                        removeRandomIndices_anchorResult);
                                                        removeFromArrayList(arrayList, size / 2,
                                                                        removeRandomIndices_arrayListResult);
                                                        result = valueCheck(anchor, arrayList, size / 2,
                                                                        get_after_removeRandomIndices_anchorResult,
                                                                        get_after_removeRandomIndices_arrayListResult);
                                                        break;

                                                case 6:
                                                        /*
                                                         * System.out.println(
                                                         * "\n7: test anchor list vs arrayList half push_front then push_back the other half, then half pop_front then pop_back the other half"
                                                         * );
                                                         */
                                                        anchor = new AnchorList<>();
                                                        arrayList = new ArrayList<>();
                                                        for (int i = 0; i < size / 2; i++) {
                                                                indexes[i] = 0;
                                                                indexes[i + size / 2] = i + size / 2;
                                                        }
                                                        insertToAnchorList(anchor, size,
                                                                        pushFront_pushBack_anchorResult);
                                                        insertToArrayList(arrayList, size,
                                                                        pushFront_pushBack_arrayListResult);

                                                        result = valueCheck(anchor, arrayList, size,
                                                                        get_after_pushFront_pushBack_anchorResult,
                                                                        get_after_pushFront_pushBack_arrayListResult);
                                                        if (!result) {
                                                                break;
                                                        }
                                                        for (int i = 0; i < size / 4; i++) {
                                                                deleteIndexes[i] = 0;
                                                                deleteIndexes[i + size / 4] = size - (i + size / 4 + 1);
                                                        }
                                                        removeFromAnchorList(anchor, size / 2,
                                                                        popFront_popBack_anchorResult);
                                                        removeFromArrayList(arrayList, size / 2,
                                                                        popFront_popBack_arrayListResult);

                                                        result = valueCheck(anchor, arrayList, size / 2,
                                                                        get_after_popFront_popBack_anchorResult,
                                                                        get_after_popFront_popBack_arrayListResult);
                                                        break;

                                                case 7:
                                                        /*
                                                         * System.out.println(
                                                         * "\n8: test anchor list vs arrayList half push_back then push_front the other half, then half pop_back then pop_front the other half"
                                                         * );
                                                         */
                                                        anchor = new AnchorList<>();
                                                        arrayList = new ArrayList<>();
                                                        for (int i = 0; i < size / 2; i++) {
                                                                indexes[i] = i;
                                                                indexes[i + size / 2] = 0;
                                                        }
                                                        insertToAnchorList(anchor, size,
                                                                        pushBack_pushFront_anchorResult);
                                                        insertToArrayList(arrayList, size,
                                                                        pushBack_pushFront_arrayListResult);
                                                        result = valueCheck(anchor, arrayList, size,
                                                                        get_after_pushBack_pushFront_anchorResult,
                                                                        get_after_pushBack_pushFront_arrayListResult);
                                                        if (!result) {
                                                                break;
                                                        }
                                                        for (int i = 0; i < size / 4; i++) {
                                                                deleteIndexes[i] = size - (i + 1);
                                                                deleteIndexes[i + size / 4] = 0;
                                                        }
                                                        removeFromAnchorList(anchor, size / 2,
                                                                        popBack_popFront_anchorResult);
                                                        removeFromArrayList(arrayList, size / 2,
                                                                        popBack_popFront_arrayListResult);

                                                        result = valueCheck(anchor, arrayList, size / 2,
                                                                        get_after_popBack_popFront_anchorResult,
                                                                        get_after_popBack_popFront_arrayListResult);
                                                        break;
                                        }

                                        if (result) {
                                                // System.out.println("** That was correct!");
                                        } else {
                                                // System.out.println("** Doesn't match.");
                                                // System.out.println("Your code did not pass the tests.");
                                                return;
                                        }
                                }
                        }
                }
                // Save all timing results
                saveMapToCSV(pushBack_anchorResult, "timeTakenResults/pushBack_anchor_results.csv");
                saveMapToCSV(get_after_pushBack_anchorResult, "timeTakenResults/get_after_pushBack_anchor_results.csv");

                saveMapToCSV(popBackAfterPushBack_anchorResult,
                                "timeTakenResults/popBackAfterPushBack_anchor_results.csv");
                saveMapToCSV(get_after_popBackAfterPushBack_anchorResult,
                                "timeTakenResults/get_after_popBackAfterPushBack_anchor_results.csv");

                saveMapToCSV(popFrontAfterPushBack_anchorResult,
                                "timeTakenResults/popFrontAfterPushBack_anchor_results.csv");
                saveMapToCSV(get_after_popFrontAfterPushBack_anchorResult,
                                "timeTakenResults/get_after_popFrontAfterPushBack_anchor_results.csv");

                saveMapToCSV(removeRandomIndicesAfterPushBack_anchorResult,
                                "timeTakenResults/removeRandomIndicesAfterPushBack_anchor_results.csv");
                saveMapToCSV(get_after_removeRandomIndicesAfterPushBack_anchorResult,
                                "timeTakenResults/get_after_removeRandomIndicesAfterPushBack_anchor_results.csv");

                saveMapToCSV(pushBack_arrayListResult, "timeTakenResults/pushBack_arraylist_results.csv");
                saveMapToCSV(get_after_pushBack_arrayListResult,
                                "timeTakenResults/get_after_pushBack_arraylist_results.csv");

                saveMapToCSV(popBackAfterPushBack_arrayListResult,
                                "timeTakenResults/popBackAfterPushBack_arraylist_results.csv");
                saveMapToCSV(get_after_popBackAfterPushBack_arrayListResult,
                                "timeTakenResults/get_after_popBackAfterPushBack_arraylist_results.csv");

                saveMapToCSV(popFrontAfterPushBack_arrayListResult,
                                "timeTakenResults/popFrontAfterPushBack_arraylist_results.csv");
                saveMapToCSV(get_after_popFrontAfterPushBack_arrayListResult,
                                "timeTakenResults/get_after_popFrontAfterPushBack_arraylist_results.csv");

                saveMapToCSV(removeRandomIndicesAfterPushBack_arrayListResult,
                                "timeTakenResults/removeRandomIndicesAfterPushBack_arraylist_results.csv");
                saveMapToCSV(get_after_removeRandomIndicesAfterPushBack_arrayListResult,
                                "timeTakenResults/get_after_removeRandomIndicesAfterPushBack_arraylist_results.csv");

                saveMapToCSV(pushFront_anchorResult, "timeTakenResults/pushFront_anchor_results.csv");
                saveMapToCSV(get_after_pushFront_anchorResult,
                                "timeTakenResults/get_after_pushFront_anchor_results.csv");

                saveMapToCSV(popFrontAfterPushFront_anchorResult,
                                "timeTakenResults/popFrontAfterPushFront_anchor_results.csv");
                saveMapToCSV(get_after_popFrontAfterPushFront_anchorResult,
                                "timeTakenResults/get_after_popFrontAfterPushFront_anchor_results.csv");

                saveMapToCSV(popBackAfterPushFront_anchorResult,
                                "timeTakenResults/popBackAfterPushFront_anchor_results.csv");
                saveMapToCSV(get_after_popBackAfterPushFront_anchorResult,
                                "timeTakenResults/get_after_popBackAfterPushFront_anchor_results.csv");

                saveMapToCSV(pushFront_arrayListResult, "timeTakenResults/pushFront_arraylist_results.csv");
                saveMapToCSV(get_after_pushFront_arrayListResult,
                                "timeTakenResults/get_after_pushFront_arraylist_results.csv");

                saveMapToCSV(popFrontAfterPushFront_arrayListResult,
                                "timeTakenResults/popFrontAfterPushFront_arraylist_results.csv");
                saveMapToCSV(get_after_popFrontAfterPushFront_arrayListResult,
                                "timeTakenResults/get_after_popFrontAfterPushFront_arraylist_results.csv");

                saveMapToCSV(popBackAfterPushFront_arrayListResult,
                                "timeTakenResults/popBackAfterPushFront_arraylist_results.csv");
                saveMapToCSV(get_after_popBackAfterPushFront_arrayListResult,
                                "timeTakenResults/get_after_popBackAfterPushFront_arraylist_results.csv");

                saveMapToCSV(insertRandomIndices_anchorResult,
                                "timeTakenResults/insertRandomIndices_anchor_results.csv");
                saveMapToCSV(get_after_insertRandomIndices_anchorResult,
                                "timeTakenResults/get_after_insertRandomIndices_anchor_results.csv");

                saveMapToCSV(removeRandomIndices_anchorResult,
                                "timeTakenResults/removeRandomIndices_anchor_results.csv");
                saveMapToCSV(get_after_removeRandomIndices_anchorResult,
                                "timeTakenResults/get_after_removeRandomIndices_anchor_results.csv");

                saveMapToCSV(insertRandomIndices_arrayListResult,
                                "timeTakenResults/insertRandomIndices_arraylist_results.csv");
                saveMapToCSV(get_after_insertRandomIndices_arrayListResult,
                                "timeTakenResults/get_after_insertRandomIndices_arraylist_results.csv");

                saveMapToCSV(removeRandomIndices_arrayListResult,
                                "timeTakenResults/removeRandomIndices_arraylist_results.csv");
                saveMapToCSV(get_after_removeRandomIndices_arrayListResult,
                                "timeTakenResults/get_after_removeRandomIndices_arraylist_results.csv");

                saveMapToCSV(pushFront_pushBack_anchorResult, "timeTakenResults/pushFront_pushBack_anchor_results.csv");
                saveMapToCSV(get_after_pushFront_pushBack_anchorResult,
                                "timeTakenResults/get_after_pushFront_pushBack_anchor_results.csv");

                saveMapToCSV(popFront_popBack_anchorResult, "timeTakenResults/popFront_popBack_anchor_results.csv");
                saveMapToCSV(get_after_popFront_popBack_anchorResult,
                                "timeTakenResults/get_after_popFront_popBack_anchor_results.csv");

                saveMapToCSV(pushFront_pushBack_arrayListResult,
                                "timeTakenResults/pushFront_pushBack_arraylist_results.csv");
                saveMapToCSV(get_after_pushFront_pushBack_arrayListResult,
                                "timeTakenResults/get_after_pushFront_pushBack_arraylist_results.csv");

                saveMapToCSV(popFront_popBack_arrayListResult,
                                "timeTakenResults/popFront_popBack_arraylist_results.csv");
                saveMapToCSV(get_after_popFront_popBack_arrayListResult,
                                "timeTakenResults/get_after_popFront_popBack_arraylist_results.csv");

                saveMapToCSV(pushBack_pushFront_anchorResult, "timeTakenResults/pushBack_pushFront_anchor_results.csv");
                saveMapToCSV(get_after_pushBack_pushFront_anchorResult,
                                "timeTakenResults/get_after_pushBack_pushFront_anchor_results.csv");

                saveMapToCSV(popBack_popFront_anchorResult, "timeTakenResults/popBack_popFront_anchor_results.csv");
                saveMapToCSV(get_after_popBack_popFront_anchorResult,
                                "timeTakenResults/get_after_popBack_popFront_anchor_results.csv");

                saveMapToCSV(pushBack_pushFront_arrayListResult,
                                "timeTakenResults/pushBack_pushFront_arraylist_results.csv");
                saveMapToCSV(get_after_pushBack_pushFront_arrayListResult,
                                "timeTakenResults/get_after_pushBack_pushFront_arraylist_results.csv");

                saveMapToCSV(popBack_popFront_arrayListResult,
                                "timeTakenResults/popBack_popFront_arraylist_results.csv");
                saveMapToCSV(get_after_popBack_popFront_arrayListResult,
                                "timeTakenResults/get_after_popBack_popFront_arraylist_results.csv");

                System.out.println("Your code passed all the tests.");
        }
}