import pcbnew as pn
import os
board_name = "trainCatcher"
board = pn.LoadBoard("trainCatcher.kicad_pcb")
pctl = pn.PLOT_CONTROLLER(board)
enabled_layers = board.GetEnabledLayers()

layer_ids = list(enabled_layers.Seq())

layer_names = []
for layer_id in layer_ids:
    layer_names.append(board.GetLayerName(layer_id))
max_string_len = max(layer_names, key=len)

for i, layer_id in enumerate(layer_ids):
    pctl.SetLayer(layer_id)
    layer_name = board.GetLayerName(layer_id).replace(".", "_")
    layer_filename = os.path.join(board_name + ".gbr")
    pctl.OpenPlotfile(plot_sufix, pn.PLOT_FORMAT_GERBER, layer_name)
    pctl.PlotLayer()