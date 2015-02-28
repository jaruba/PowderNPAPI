# Powder NPAPI Plugin
Peerflix interface for the browser. (NPAPI Plugin made with FireBreath + Node.js + Peerflix) - [SEE DEMO](http://jaruba.github.io/PowderNPAPI/)

**Embedding in page:**

    <object id="powder" type="application/x-powdernpapi" width="1" height="1"></object>


JS API
==============

**Start Peerflix** - requires the info hash from a magnet link, returns a stream id (int). The streaming link will be returned in the "torrentData" event.

    .getStream(infoHash)

**Request Data** - requires a stream id, returns "true" if new data is available and "false" if no new data is available. This function does not return the data in order to not freeze the page while waiting for a response, the data will come when the "torrentData" event is triggered.

    .requestData(streamId)

**Read Data** - event that returns a stream id and json data from peerflix. The JSON Object can contain: runtime, path, uploaded, downloaded, peerqueue, seeds, link, filename, size.

    torrentData(streamId,jsonData)

**Kill Stream** - terminates a process by stream id.

    .killStream(streamId)

**Kill All** - terminates all peerflix processes.

    .killAll()
