package emergentworlds.world.cell.cells;

import emergentworlds.world.World;
import emergentworlds.world.cell.Cell;
import emergentworlds.world.cell.CellType;

public class DirtCell extends Cell {
    public DirtCell() {
        super();
        this._type = CellType.DIRT;
        _cellRenderer.setColor(new float[]{102/255.f, 58/255.f, 27/255.f, 1.0f});
    }

}
