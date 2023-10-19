package arkengine.util;

public class Time {
    // Initialized at application startup
    public static double timeStarted = System.nanoTime() * 1E-9;

    public static double getTime() {
        return (double)((System.nanoTime() - timeStarted) * 1E-9);
    }
}
