package emergentworlds.world.cell.cells;

import emergentworlds.world.World;
import emergentworlds.world.cell.Cell;

/**
 * An empty cell with absolutely nothing in it
 */
public class VoidCell extends Cell {
    public VoidCell() {
        super();
        _cellRenderer.setColor(new float[]{0, 0, 0, 0});
    }
}
